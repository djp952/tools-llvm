#__tools-llvm__
Managed C++/CLI wrappers to use [__LLVM__](http://llvm.org/) and its constituent subprojects from the Microsoft.NET CLR environment (C#/VB.NET/etc).  Currently only includes support for the [__Clang__](http://clang.llvm.org/) subproject, but may include additional projects in the future (stranger things have been known to happen)  
  
Copyright (C)2016 Michael G. Brehm    
[MIT LICENSE](https://opensource.org/licenses/MIT)

##__MODULES__
__zuki.tools.llvm.clang__ - Managed code wrapper/bindings for [__Clang__](http://clang.llvm.org/): a C language family frontend for LLVM  
__zuki.tools.llvm.clang.samples__ - Sample code for zuki.tools.llvm.clang  
__zuki.tools.llvm.clang.test__ - Unit tests for zuki.tools.llvm.clang  

##__NUGET PACKAGES__
zuki.tools.llvm.clang is available as a public nuget package under the name 'zClang'. I also started working on some simple samples to help illustrate how to use the library:  
   
[__zClang__](https://www.nuget.org/packages/zuki.tools.llvm.clang/)  
[__zClang Samples__](https://www.nuget.org/packages/zuki.tools.llvm.clang.Samples/)
```
PM> Install-Package zuki.tools.llvm.clang
PM> Install-Package zuki.tools.llvm.clang.Samples   
```

##__DOCUMENTATION__
You're looking at it :) If there is any community interest for this project I'll consider writing some. Almost the entire libclang API (see __LIBCLANG API CALLS NOT IMPLEMENTED__ below for the exceptions) can be accessed through this library. I tried to make the object model relatively intuitive and easy to follow. You can always look at the unit tests for examples and get a feel for the overall breadth of this library. Everything starts with the static methods of the Clang object and cascade from there.  Please open an Issue if there are any specific (and relatively straightforward) examples that you would like to see generated that would add documentation value, and I'll add them to the Samples nuget package:
```
using zuki.tools.llvm.clang;
using zuki.tools.llvm.clang.extensions;

// C# - TranslationUnit indexing
using(TranslationUnit tu = Clang.CreateTranslationUnit(mycppfile, TranslationUnitParseOptions.DetailedPreprocessingRecord)
{
    // do fun and interesting translation unit stuff
}

// C# - Callback based indexing
using(Index index = Clang.CreateIndex())
{
    // do fun and interesting callback-based indexer stuff
    index.StartedTranslationUnit += MyIndexStarted;
    index.Declaration += MyIndexDeclarationHandler;
    index.EntityReference += MyIndexEntityReference;
    index.IndexSourceFile(mycppfile, mystateobject, IndexOptions.None)
}
```
## __BUILD__
###__System Requirements__
* Microsoft Visual Studio 2015 Community Edition (or higher)

###__Source Code Repositories__
[__djp952/build__](https://github.com/djp952/build) - build tools  
[__djp952/external-libclang__](https://github.com/djp952/external-libclang) - libclang headers and binaries (patched - see __IMPLEMENTATION NOTES__ below)  
[__djp952/tools-llvm__](https://github.com/djp952/tools-llvm) - this repository  
```
git clone https://github.com/djp952/build  
git clone https://github.com/djp952/external-libclang  
git clone https://github.com/djp952/tools-llvm
```
###__Build Win32 release binaries__  
Open Developer Command Prompt for Visual Studio  
```
devenv tools-llvm.sln /Build "Release|Win32" /project clang
  
> out\Win32\Release\zuki.tools.llvm.clang.dll
> out\Win32\Release\libclang.dll
```
###__Build x64 release binaries__
Open Developer Command Prompt for Visual Studio  
```
devenv tools-llvm.sln /Build "Release|x64" /project clang

> out\x64\Release\zuki.tools.llvm.clang.dll
> out\x64\Release\libclang.dll
```
##__IMPLEMENTATION NOTES__

###__Not Supported: Reparsing Translation Units__
Reparsing a translation unit does not invalidate the original CXTranslationUnit pointer/handle from libclang, but all existing objects (i.e. Cursors, Files, etc.) that refer to the original translation unit become defacto invalidated.  If the reparse operation fails for any reason, the original translation unit is still invalidated and can only be disposed of.  This library uses custom CLR SafeHandles to maintain the lifetime of disposable libclang objects like translation units and special reference handles for non-disposable objects that refer into them.  To support reparse a secondary lifetime management layer would need to be implemented to track if a translation unit SafeHandle is still valid (not disposed) but was reparsed, making any objects that existed prior to that reparse operation invalid.  Implementing a method like "TranslationUnit.Reparse()" would therefore always unconditionally invalidate/dispose all existing objects that refer into the translation unit, regardless of success or failure, and that's a pretty horrible side effect for managed code callers to deal with. Given that there is little difference between reparsing an existing translation unit and explicitly destroying it and subsequently creating a new one within the Index I opted to simply not support reparsing translation units.  If this is a major problem for anyone please open an Issue -- if there is appreciable value that would be added by supporting this I will look into adding it.

###__Not Supported: Retrieval of the Translation Unit instance generated by the callback-based Indexer__
libclang allows for retrieval of a translation unit generated by an indexer (callback-based) operation executed against a source file.  This library does not request generation of or subsequently expose that resultant translation unit to the caller.  It was neither difficult nor problematic to do so, but a handful of unit test operations executed against that translation unit fail; there is some level of differing/invalid internal state.  If in the future libclang produces a functionally equivalent translation unit from the indexer, this support can be added. Creating a translation unit and then passing it through the Indexer via Index.IndexTranslationUnit() has no such limitations. 

###__Partially Supported: Compilation Database Source Mappings__
libclang does not support source mappings with its default JSON database implementation.  As a result, CompileCommand objects will always have an empty source mappings collection.  The underlying code to access database source mappings exists and theoretically would populate correctly if you use a custom libclang.dll that has them.  Please open an Issue with a link to the libclang.dll version you are using if you expect compile command source mappings to work and they do not. 

###__Workaround: Set LIBCLANG_NOTHREADS when unit testing/debugging Indexer callbacks__
libclang uses a worker thread to perform callback-based indexing by default [Index.IndexSourceFile() / Index.IndexTranslationUnit()].  When executed within the Visual Studio managed code debugger the callbacks from the libclang thread that trigger the events of the Index object will fail.  In order to successfully debug or unit test a project that uses the Indexer functions, the LIBCLANG_NOTHREADS environment variable must first be set to prevent libclang from creating the worker thread(s).

###__Workaround: Invalid JSON input to Compilation Database causes loader lock__
If an invalid/corrupt JSON file is referenced when creating a compilation database [Clang.CreateCompilationDatabase()], a possible Loader Lock condition can occur from libclang.  As a workaround, this library will attempt to parse the input JSON using the .NET DataContractJsonSerializer object to ensure it's syntactical correctnes prior to invoking the libclang API.

###__Workaround: Tokenizing extent may generate an extra token__ (https://llvm.org/bugs/show_bug.cgi?id=9069)
There is a disconnect in libclang regarding source ranges (extents) and what tokens are considered part of that source range (see linked bug 9069 for more information).  As a workaround, this library will check the locaton of the last token generated by Extent.GetTokens() and remove it from the resultant collection if that location does not fall within the Extent.

###__Patch: Double release of unsaved files during clang_indexSourceFile__ (https://llvm.org/bugs/show_bug.cgi?id=11864)
libclang has a known defect/bug that causes a double release of the memory associated with unsaved files passed into the Indexer [Index.IndexSourceFile()]. Calling the Indexer with unsaved files on 64-bit builds will cause a heap corruption exception and terminate the process.  I patched the defect in libclang.dll as there was no way to implement a workaround for this in the managed code layer. Using the official version of libclang.dll or another customized version that hasn't patched this concern may result in an inability to use the Indexer with unsaved files through this library.

##__LIBCLANG API CALLS NOT IMPLEMENTED__
The following is a list of functions exported by libclang.dll that are not used by or accessible from this library:

###__clang_getInstantiationLocation__
* Deprecated in libclang; superseded by clang_getExpansionLocation

###__clang_getDiagnosticSetFromTU__
* Unnecessary; the translation unit diagnostic collection is accessed via clang_getNumDiagnostics and clang_getDiagnostic 
 
###__clang_getDiagnosticCategoryName__
* Deprecated in libclang; superseded by clang_getDiagnosticCategoryText

###__clang_createTranslationUnitFromSourceFile__
* Unnecessary; this is functionally equivalent to calling clang_parseTranslationUnit with an option mask of CXTranslationUnit_DetailedPreprocessingRecord 

###__clang_createTranslationUnit__
* Unnecessary; this is functionally equivalent to calling clang_createTranslationUnit2

###__clang_parseTranslationUnit__
* Unnecessary; this is functionally equivlalent to calling clang_parseTranslationUnit2

###__clang_defaultReparseOptions__
* Not supported; translation unit reparse operations are not available in this library (see IMPLEMENTATION NOTES above)

###__clang_reparseTranslationUnit__
* Not supported; translation unit reparse operations are not available in this library (see IMPLEMENTATION NOTES above)

###__clang_Cursor_getTranslationUnit__
* Unnecessary; the translation unit associated with a cursor is maintained internally by this library

###__clang_createCXCursorSet__ 
* Unnecessary; managed collection classes are used to generate sets of cursor instances

###__clang_disposeCXCursorSet__ 
* Unnecessary; managed collection classes are used to generate sets of cursor instances

###__clang_CXCursorSet_contains__ 
* Unnecessary; managed collection classes are used to generate sets of cursor instances

###__clang_CXCursorSet_insert__
* Unnecessary; managed collection classes are used to generate sets of cursor instances

###__clang_getDefinitionSpellingAndExtent__ 
* Marked as experimental by libclang

###__clang_enableStackTraces__ 
* Marked as experimental by libclang

###__clang_executeOnThread__
* Marked as experimental by libclang

###__clang_indexLoc_getFileLocation__
* Unnecessary; indexer locations are retrieved with clang_indexLoc_getCXSourceLocation

