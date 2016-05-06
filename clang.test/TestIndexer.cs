//---------------------------------------------------------------------------
// Copyright (c) 2016 Michael G. Brehm
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------

using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestIndexer
	{
		/// <summary>
		/// Dummy object to apply as indexer state
		/// </summary>
		class IndexerState
		{
			public IndexerState() { }
		};

		/// <summary>
		/// Exercises an IndexAttributeCollection instance
		/// </summary>
		/// <param name="attributes">IndexAttributeCollection to be exercised</param>
		private void ExerciseIndexAttributes(IndexAttributeCollection attributes)
		{
			Assert.IsNotNull(attributes);

			if (attributes.Count > 0)
			{
				Assert.IsNotNull(attributes[0]);
				Assert.AreSame(attributes[0], attributes[0]);
			}

			foreach (IndexAttribute attribute in attributes)
			{
				Assert.IsNotNull(attribute.Cursor);
				Assert.AreSame(attribute.Cursor, attribute.Cursor);
				Assert.IsFalse(Cursor.IsNull(attribute.Cursor));

				Assert.IsNotNull(attribute.Location);
				Assert.AreSame(attribute.Location, attribute.Location);
				Assert.IsFalse(Location.IsNull(attribute.Location));

				// IBOutletCollection attributes can be exercised further
				if (attribute.Kind == IndexAttributeKind.IBOutletCollection)
				{
					IndexIBOutletCollectionAttribute ibocattribute = (IndexIBOutletCollectionAttribute)attribute;
					Assert.IsNotNull(ibocattribute);
					Assert.IsTrue(ibocattribute.Kind == IndexAttributeKind.IBOutletCollection);

					Assert.IsNotNull(ibocattribute.ClassCursor);
					Assert.AreSame(ibocattribute.ClassCursor, ibocattribute.ClassCursor);
					Assert.IsFalse(Cursor.IsNull(ibocattribute.ClassCursor));

					Assert.IsNotNull(ibocattribute.ClassEntity);
					Assert.AreSame(ibocattribute.ClassEntity, ibocattribute.ClassEntity);
					ExerciseIndexEntity(ibocattribute.ClassEntity);

					Assert.IsNotNull(ibocattribute.ClassLocation);
					Assert.AreSame(ibocattribute.ClassLocation, ibocattribute.ClassLocation);
					Assert.IsFalse(Location.IsNull(ibocattribute.ClassLocation));
				}

				// exercise the generic enumerator for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)attributes) Assert.IsNotNull(obj);
			}
		}

		/// <summary>
		/// Exercises an IndexEntity instance
		/// </summary>
		/// <param name="entity">IndexEntity to be exercised</param>
		private void ExerciseIndexEntity(IndexEntity entity)
		{
			Assert.IsNotNull(entity);

			// There are legitimate reasons that an entity can be null, check for that
			// case first and ensure all the properties return as expected
			if (IndexEntity.IsNull(entity))
			{
				Assert.IsNotNull(entity.Attributes);
				Assert.AreSame(entity.Attributes, entity.Attributes);
				Assert.AreEqual(0, entity.Attributes.Count);
				Assert.IsNull(entity.ClientData);
				Assert.IsNotNull(entity.Cursor);
				Assert.AreSame(entity.Cursor, entity.Cursor);
				Assert.IsTrue(Cursor.IsNull(entity.Cursor));
				Assert.AreEqual(IndexEntityCxxTemplateKind.NonTemplate, entity.CxxTemplateKind);
				Assert.AreEqual(IndexEntityKind.Unexposed, entity.Kind);
				Assert.AreEqual(IndexEntityLanguage.None, entity.Language);
				Assert.IsNotNull(entity.Name);
				Assert.AreSame(entity.Name, entity.Name);
				Assert.AreEqual(String.Empty, entity.Name);
				Assert.AreEqual(entity.Name, entity.ToString());
				Assert.IsNotNull(entity.UnifiedSymbolResolution);
				Assert.AreSame(entity.UnifiedSymbolResolution, entity.UnifiedSymbolResolution);
				Assert.AreEqual(String.Empty, entity.UnifiedSymbolResolution);
				return;
			}

			Assert.IsNotNull(entity.Attributes);
			Assert.AreSame(entity.Attributes, entity.Attributes);
			ExerciseIndexAttributes(entity.Attributes);

			// If a client data object was assigned, check it
			if (entity.ClientData != null)
			{
				Assert.IsInstanceOfType(entity.ClientData, typeof(string));
				Assert.AreEqual("entity_" + entity.Name, (string)entity.ClientData);
			}

			Assert.IsNotNull(entity.Cursor);
			Assert.AreSame(entity.Cursor, entity.Cursor);
			Assert.IsFalse(Cursor.IsNull(entity.Cursor));

			// Check a couple specific entities from the input file for CxxTemplateKind
			if (entity.Name == "MyTemplate") Assert.AreEqual(IndexEntityCxxTemplateKind.Template, entity.CxxTemplateKind);
			else if(entity.Name == "I2") Assert.AreEqual(IndexEntityCxxTemplateKind.NonTemplate, entity.CxxTemplateKind);

			// Check a couple specific entities from the input file for Language
			if (entity.Name == "BaseClass") Assert.AreEqual(IndexEntityLanguage.CPlusPlus, entity.Language);
			else if (entity.Name == "I2") Assert.AreEqual(IndexEntityLanguage.ObjectiveC, entity.Language);

			Assert.IsNotNull(entity.Name);
			Assert.AreSame(entity.Name, entity.Name);

			Assert.AreEqual(entity.Name, entity.ToString());

			Assert.IsNotNull(entity.UnifiedSymbolResolution);
			Assert.AreSame(entity.UnifiedSymbolResolution, entity.UnifiedSymbolResolution);
			Assert.IsFalse(String.IsNullOrEmpty(entity.UnifiedSymbolResolution));

			// Set a custom entity object, make sure it doesn't throw an exception
			if (entity.ClientData == null) entity.ClientData = "entity_" + entity.Name;
		}

		/// <summary>
		/// Exercises an IndexCxxClassDeclaration instance
		/// </summary>
		/// <param name="declaration">IndexCxxClassDeclaration to be exercised</param>
		private void ExerciseCxxClassDeclaration(IndexCxxClassDeclaration declaration)
		{
			Assert.IsNotNull(declaration);
			Assert.AreEqual(IndexDeclarationKind.CxxClass, declaration.Kind);

			Assert.IsNotNull(declaration.BaseClasses);
			Assert.AreSame(declaration.BaseClasses, declaration.BaseClasses);

			if (declaration.BaseClasses.Count > 0)
			{
				Assert.IsNotNull(declaration.BaseClasses[0]);
				Assert.AreSame(declaration.BaseClasses[0], declaration.BaseClasses[0]);
			}

			foreach (IndexBaseClass baseclass in declaration.BaseClasses)
			{
				Assert.IsNotNull(baseclass);
				Assert.IsNotNull(baseclass.BaseEntity);
				Assert.AreSame(baseclass.BaseEntity, baseclass.BaseEntity);
				Assert.IsNotNull(baseclass.Cursor);
				Assert.IsFalse(Cursor.IsNull(baseclass.Cursor));
				Assert.IsNotNull(baseclass.Location);
				Assert.IsFalse(Location.IsNull(baseclass.Location));
			}

			// exercise the generic enumerator for code coverage
			foreach (Object obj in (System.Collections.IEnumerable)declaration.BaseClasses) Assert.IsNotNull(obj);
		}

		/// <summary>
		/// Exercises an IndexObjectiveCCategoryDeclaration instance
		/// </summary>
		/// <param name="declaration">IndexObjectiveCCategoryDeclaration instance to be exercised</param>
		private void ExerciseObjectiveCCategoryDeclaration(IndexObjectiveCCategoryDeclaration declaration)
		{
			Assert.IsNotNull(declaration);
			Assert.AreEqual(IndexDeclarationKind.ObjectiveCCategory, declaration.Kind);

			Assert.IsNotNull(declaration.ClassCursor);
			Assert.AreSame(declaration.ClassCursor, declaration.ClassCursor);
			Assert.IsFalse(Cursor.IsNull(declaration.ClassCursor));

			Assert.IsNotNull(declaration.ClassEntity);
			Assert.AreSame(declaration.ClassEntity, declaration.ClassEntity);
			ExerciseIndexEntity(declaration.ClassEntity);

			Assert.IsNotNull(declaration.ClassLocation);
			Assert.AreSame(declaration.ClassLocation, declaration.ClassLocation);
			Assert.IsFalse(Location.IsNull(declaration.ClassLocation));

			Assert.IsNotNull(declaration.ProtocolReferences);
			Assert.AreSame(declaration.ProtocolReferences, declaration.ProtocolReferences);

			if (declaration.ProtocolReferences.Count > 0)
			{
				Assert.IsNotNull(declaration.ProtocolReferences[0]);
				Assert.AreSame(declaration.ProtocolReferences[0], declaration.ProtocolReferences[0]);
			}

			foreach (IndexObjectiveCProtocolReference protocolref in declaration.ProtocolReferences)
			{
				Assert.IsNotNull(protocolref);

				Assert.IsNotNull(protocolref.Cursor);
				Assert.AreSame(protocolref.Cursor, protocolref.Cursor);
				Assert.IsFalse(Cursor.IsNull(protocolref.Cursor));

				Assert.IsNotNull(protocolref.Location);
				Assert.AreSame(protocolref.Location, protocolref.Location);
				Assert.IsFalse(Location.IsNull(protocolref.Location));

				Assert.IsNotNull(protocolref.ProtocolEntity);
				Assert.AreSame(protocolref.ProtocolEntity, protocolref.ProtocolEntity);
				ExerciseIndexEntity(protocolref.ProtocolEntity);
			}

			// exercise the generic enumerator for code coverage
			foreach (Object obj in (System.Collections.IEnumerable)declaration.ProtocolReferences) Assert.IsNotNull(obj);
		}

		/// <summary>
		/// Exercises an IndexObjectiveCContainerDeclaration instance
		/// </summary>
		/// <param name="declaration">IndexObjectiveCContainerDeclaration instance to be exercised</param>
		private void ExerciseObjectiveCContainerDeclaration(IndexObjectiveCContainerDeclaration declaration)
		{
			Assert.IsNotNull(declaration);
			Assert.AreEqual(IndexDeclarationKind.ObjectiveCContainer, declaration.Kind);

			// Check a couple specific known container kinds from the input file
			if (IndexEntity.IsNull(declaration.Entity)) return;
			else if (declaration.Entity.Name == "Foo") Assert.AreEqual(IndexObjectiveCContainerKind.ForwardRef, declaration.ContainerKind);
			else if (declaration.Entity.Name == "I2") Assert.AreEqual(IndexObjectiveCContainerKind.Implementation, declaration.ContainerKind);
		}

		/// <summary>
		/// Exercises an IndexObjectiveCInterfaceDeclaration instance
		/// </summary>
		/// <param name="declaration">IndexObjectiveCInterfaceDeclaration instance to be exercised</param>
		private void ExerciseObjectiveCInterfaceDeclaration(IndexObjectiveCInterfaceDeclaration declaration)
		{
			Assert.IsNotNull(declaration);
			Assert.AreEqual(IndexDeclarationKind.ObjectiveCInterface, declaration.Kind);

			Assert.IsNotNull(declaration.ProtocolReferences);
			Assert.AreSame(declaration.ProtocolReferences, declaration.ProtocolReferences);

			if (declaration.ProtocolReferences.Count > 0)
			{
				Assert.IsNotNull(declaration.ProtocolReferences[0]);
				Assert.AreSame(declaration.ProtocolReferences[0], declaration.ProtocolReferences[0]);
			}

			foreach (IndexObjectiveCProtocolReference protocolref in declaration.ProtocolReferences)
			{
				Assert.IsNotNull(protocolref);

				Assert.IsNotNull(protocolref.Cursor);
				Assert.AreSame(protocolref.Cursor, protocolref.Cursor);
				Assert.IsFalse(Cursor.IsNull(protocolref.Cursor));

				Assert.IsNotNull(protocolref.Location);
				Assert.AreSame(protocolref.Location, protocolref.Location);
				Assert.IsFalse(Location.IsNull(protocolref.Location));

				Assert.IsNotNull(protocolref.ProtocolEntity);
				Assert.AreSame(protocolref.ProtocolEntity, protocolref.ProtocolEntity);
				ExerciseIndexEntity(protocolref.ProtocolEntity);
			}

			// exercise the generic enumerator for code coverage
			foreach (Object obj in (System.Collections.IEnumerable)declaration.ProtocolReferences) Assert.IsNotNull(obj);

			Assert.IsNotNull(declaration.SuperClass);
			Assert.AreSame(declaration.SuperClass, declaration.SuperClass);

			if (!IndexBaseClass.IsNull(declaration.SuperClass))
			{
				Assert.IsNotNull(declaration.SuperClass.Cursor);
				Assert.AreSame(declaration.SuperClass.Cursor, declaration.SuperClass.Cursor);
				Assert.IsFalse(Cursor.IsNull(declaration.SuperClass.Cursor));

				Assert.IsNotNull(declaration.SuperClass.BaseEntity);
				Assert.AreSame(declaration.SuperClass.BaseEntity, declaration.SuperClass.BaseEntity);
				ExerciseIndexEntity(declaration.SuperClass.BaseEntity);

				Assert.IsNotNull(declaration.SuperClass.Location);
				Assert.AreSame(declaration.SuperClass.Location, declaration.SuperClass.Location);
				Assert.IsFalse(Location.IsNull(declaration.SuperClass.Location));
			}
		}

		/// <summary>
		/// Exercises an IndexObjectiveCPropertyDeclaration instance
		/// </summary>
		/// <param name="declaration">IndexObjectiveCPropertyDeclaration instance to be exercised</param>
		private void ExerciseObjectiveCPropertyDeclaration(IndexObjectiveCPropertyDeclaration declaration)
		{
			Assert.IsNotNull(declaration);
			Assert.AreEqual(IndexDeclarationKind.ObjectiveCProperty, declaration.Kind);

			Assert.IsNotNull(declaration.GetterEntity);
			Assert.AreSame(declaration.GetterEntity, declaration.GetterEntity);
			ExerciseIndexEntity(declaration.GetterEntity);

			Assert.IsNotNull(declaration.SetterEntity);
			Assert.AreSame(declaration.SetterEntity, declaration.SetterEntity);
			ExerciseIndexEntity(declaration.SetterEntity);
		}

		/// <summary>
		/// Exercises an IndexObjectiveCProtocolDeclaration instance
		/// </summary>
		/// <param name="declaration">IndexObjectiveCProtocolDeclaration instance to be exercised</param>
		private void ExerciseObjectiveCProtocolDeclaration(IndexObjectiveCProtocolDeclaration declaration)
		{
			Assert.IsNotNull(declaration);
			Assert.AreEqual(IndexDeclarationKind.ObjectiveCProtocol, declaration.Kind);

			Assert.IsNotNull(declaration.ProtocolReferences);
			Assert.AreSame(declaration.ProtocolReferences, declaration.ProtocolReferences);

			if (declaration.ProtocolReferences.Count > 0)
			{
				Assert.IsNotNull(declaration.ProtocolReferences[0]);
				Assert.AreSame(declaration.ProtocolReferences[0], declaration.ProtocolReferences[0]);
			}

			foreach (IndexObjectiveCProtocolReference protocolref in declaration.ProtocolReferences)
			{
				Assert.IsNotNull(protocolref);

				Assert.IsNotNull(protocolref.Cursor);
				Assert.AreSame(protocolref.Cursor, protocolref.Cursor);
				Assert.IsFalse(Cursor.IsNull(protocolref.Cursor));

				Assert.IsNotNull(protocolref.Location);
				Assert.AreSame(protocolref.Location, protocolref.Location);
				Assert.IsFalse(Location.IsNull(protocolref.Location));

				Assert.IsNotNull(protocolref.ProtocolEntity);
				Assert.AreSame(protocolref.ProtocolEntity, protocolref.ProtocolEntity);
				ExerciseIndexEntity(protocolref.ProtocolEntity);
			}

			// exercise the generic enumerator for code coverage
			foreach (Object obj in (System.Collections.IEnumerable)declaration.ProtocolReferences) Assert.IsNotNull(obj);
		}

		/// <summary>
		/// Handler for Index.Abort
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnAbort(object sender, IndexAbortEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			// Exercise the abort flag, it should start out as false
			Assert.IsFalse(args.Abort);
			args.Abort = true;
			Assert.IsTrue(args.Abort);
			args.Abort = false;
		}

		/// <summary>
		/// Handler for Index.Declaration
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnDeclaration(object sender, IndexDeclarationEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			Assert.IsNotNull(args.Declaration);
			Assert.AreSame(args.Declaration, args.Declaration);

			// Ignore declarations that aren't in the main unit test input file
			if (!args.Declaration.Location.IsFromMainFile) return;

			// Exercise all the base IndexDeclaration stuff first, .Kind is handled below
			Assert.IsNotNull(args.Declaration.Attributes);
			Assert.AreSame(args.Declaration.Attributes, args.Declaration.Attributes);
			ExerciseIndexAttributes(args.Declaration.Attributes);

			Assert.IsNotNull(args.Declaration.Cursor);
			Assert.AreSame(args.Declaration.Cursor, args.Declaration.Cursor);
			Assert.IsFalse(Cursor.IsNull(args.Declaration.Cursor));

			Assert.IsNotNull(args.Declaration.Entity);
			Assert.AreSame(args.Declaration.Entity, args.Declaration.Entity);
			ExerciseIndexEntity(args.Declaration.Entity);

			// There are no skipped declarations in the input files
			Assert.AreEqual(IndexDeclarationFlags.None, args.Declaration.Flags);

			if (args.Declaration.IsContainer)
			{
				IndexContainer container = args.Declaration.AsContainer();
				Assert.IsNotNull(container);

				Assert.IsNotNull(container.Cursor);
				Assert.AreSame(container.Cursor, container.Cursor);
				Assert.IsFalse(Cursor.IsNull(container.Cursor));

				// Set a custom name for the container as the client data
				container.ClientData = "container_" + container.Cursor.DisplayName;
			}

			// Pick a couple definitions from the input files
			if (args.Declaration.Entity.Name == "BaseClass") Assert.IsTrue(args.Declaration.IsDefinition);
			else if (args.Declaration.Entity.Name == "DerivedClass") Assert.IsTrue(args.Declaration.IsDefinition);

			// Don't test IsImplicit or IsRedeclaration, but make sure they don't throw an exception
			var is_imp = args.Declaration.IsImplicit;
			var is_redec = args.Declaration.IsRedeclaration;

			Assert.IsNotNull(args.Declaration.LexicalContainer);
			Assert.AreSame(args.Declaration.LexicalContainer, args.Declaration.LexicalContainer);
			Assert.IsFalse(IndexContainer.IsNull(args.Declaration.LexicalContainer));

			Assert.IsNotNull(args.Declaration.Location);
			Assert.AreEqual(args.Declaration.Location, args.Declaration.Location);
			Assert.IsFalse(Location.IsNull(args.Declaration.Location));

			Assert.IsNotNull(args.Declaration.SemanticContainer);
			Assert.AreSame(args.Declaration.SemanticContainer, args.Declaration.SemanticContainer);
			Assert.IsFalse(IndexContainer.IsNull(args.Declaration.SemanticContainer));

			// Break up the remaining declaration tests based on the kind, it would be entirely
			// too unweildly to do them all inline
			switch (args.Declaration.Kind)
			{
				case IndexDeclarationKind.None:
					break;

				case IndexDeclarationKind.CxxClass:
					ExerciseCxxClassDeclaration((IndexCxxClassDeclaration)args.Declaration);
					break;

				case IndexDeclarationKind.ObjectiveCCategory:
					ExerciseObjectiveCCategoryDeclaration((IndexObjectiveCCategoryDeclaration)args.Declaration);
					break;

				case IndexDeclarationKind.ObjectiveCContainer:
					ExerciseObjectiveCContainerDeclaration((IndexObjectiveCContainerDeclaration)args.Declaration);
					break;

				case IndexDeclarationKind.ObjectiveCInterface:
					ExerciseObjectiveCInterfaceDeclaration((IndexObjectiveCInterfaceDeclaration)args.Declaration);
					break;

				case IndexDeclarationKind.ObjectiveCProperty:
					ExerciseObjectiveCPropertyDeclaration((IndexObjectiveCPropertyDeclaration)args.Declaration);
					break;

				case IndexDeclarationKind.ObjectiveCProtocol:
					ExerciseObjectiveCProtocolDeclaration((IndexObjectiveCProtocolDeclaration)args.Declaration);
					break;
			}
		}

		/// <summary>
		/// Handler for Index.Diagnostics
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnDiagnostics(object sender, IndexDiagnosticsEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			// Exercise the diagnostics collection
			Assert.IsNotNull(args.Diagnostics);
			Assert.AreSame(args.Diagnostics, args.Diagnostics);
			if (args.Diagnostics.Count > 0)
			{
				Assert.IsNotNull(args.Diagnostics[0]);
				Assert.AreSame(args.Diagnostics[0], args.Diagnostics[0]);
				Assert.IsNotNull(args.Diagnostics[0].Location);
				Assert.IsFalse(Location.IsNull(args.Diagnostics[0].Location));
			}

			// Exercise the collection enumerators for code coverage
			foreach (Diagnostic diag in args.Diagnostics) Assert.IsNotNull(diag);
			foreach (Object obj in (System.Collections.IEnumerable)args.Diagnostics) Assert.IsNotNull(obj);
		}

		/// <summary>
		/// Handler for Index.EnteredMainFile
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnEnteredMainFile(object sender, IndexEnteredMainFileEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			// Exercise the main file object
			Assert.IsNotNull(args.File);
			Assert.IsFalse(File.IsNull(args.File));
			Assert.IsFalse(String.IsNullOrEmpty(args.File.Name));
		}

		/// <summary>
		/// Handler for Index.EntityReference
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnEntityReference(object sender, IndexEntityReferenceEventArgs args)
		{
			// Sender should always be the Index instance
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			Assert.IsNotNull(args);
			Assert.IsNotNull(args.EntityReference);
			Assert.AreSame(args.EntityReference, args.EntityReference);

			// If the state object is not null, it should be an IndexerState dummy object
			Assert.AreSame(args.State, args.State);
			if (args.State != null) Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			Assert.IsNotNull(args.EntityReference.Container);
			Assert.AreSame(args.EntityReference.Container, args.EntityReference.Container);
			if(args.EntityReference.Container.ClientData != null)
				Assert.IsTrue(((string)args.EntityReference.Container.ClientData).StartsWith("container_"));

			Assert.IsNotNull(args.EntityReference.Cursor);
			Assert.AreSame(args.EntityReference.Cursor, args.EntityReference.Cursor);
			Assert.IsFalse(Cursor.IsNull(args.EntityReference.Cursor));

			// There are no implicit references in the input files
			Assert.AreEqual(IndexEntityReferenceKind.Direct, args.EntityReference.Kind);

			Assert.IsNotNull(args.EntityReference.Location);
			Assert.AreSame(args.EntityReference.Location, args.EntityReference.Location);
			Assert.IsFalse(Location.IsNull(args.EntityReference.Location));

			Assert.IsNotNull(args.EntityReference.ParentEntity);
			Assert.AreSame(args.EntityReference.ParentEntity, args.EntityReference.ParentEntity);
			ExerciseIndexEntity(args.EntityReference.ParentEntity);

			Assert.IsNotNull(args.EntityReference.ReferencedEntity);
			Assert.AreSame(args.EntityReference.ReferencedEntity, args.EntityReference.ReferencedEntity);
			ExerciseIndexEntity(args.EntityReference.ReferencedEntity);
		}

		/// <summary>
		/// Handler for Index.ImportedASTFile
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnImportedASTFile(object sender, IndexImportedASTFileEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			// Exercise the imported ast file class
			Assert.IsNotNull(args.ImportedASTFile);
			Assert.AreSame(args.ImportedASTFile, args.ImportedASTFile);
			Assert.IsNotNull(args.ImportedASTFile.File);
			Assert.IsFalse(File.IsNull(args.ImportedASTFile.File));
			Assert.AreSame(args.ImportedASTFile.File, args.ImportedASTFile.File);
			Assert.IsTrue(args.ImportedASTFile.IsImplicit);
			Assert.IsNotNull(args.ImportedASTFile.Location);
			Assert.IsFalse(Location.IsNull(args.ImportedASTFile.Location));
			Assert.AreSame(args.ImportedASTFile.Location, args.ImportedASTFile.Location);
			Assert.IsNotNull(args.ImportedASTFile.Module);
			Assert.IsFalse(Module.IsNull(args.ImportedASTFile.Module));
			Assert.AreSame(args.ImportedASTFile.Module, args.ImportedASTFile.Module);
		}

		/// <summary>
		/// Handler for Index.IncludedFile
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnIncludedFile(object sender, IndexIncludedFileEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));

			Assert.IsNotNull(args.IncludedFile);
			Assert.AreSame(args.IncludedFile, args.IncludedFile);

			Assert.IsNotNull(args.IncludedFile.File);
			Assert.IsFalse(File.IsNull(args.IncludedFile.File));
			Assert.AreSame(args.IncludedFile.File, args.IncludedFile.File);
			Assert.IsNotNull(args.IncludedFile.Location);
			Assert.IsFalse(Location.IsNull(args.IncludedFile.Location));
			Assert.AreSame(args.IncludedFile.Location, args.IncludedFile.Location);
			Assert.IsNotNull(args.IncludedFile.Spelling);
			Assert.IsFalse(String.IsNullOrEmpty(args.IncludedFile.Spelling));
			Assert.AreSame(args.IncludedFile.Spelling, args.IncludedFile.Spelling);

			// C++ - <stdio.h>
			if (args.IncludedFile.Spelling == "stdio.h")
			{
				Assert.IsTrue(args.IncludedFile.IsAngled);
				Assert.IsFalse(args.IncludedFile.IsImport);
				Assert.IsFalse(args.IncludedFile.IsModuleImport);
			}

			// C++ - "header1.h"
			if (args.IncludedFile.Spelling == "header1.h")
			{
				Assert.IsFalse(args.IncludedFile.IsAngled);
				Assert.IsFalse(args.IncludedFile.IsImport);
				Assert.IsTrue(args.IncludedFile.IsModuleImport);
			}

			// C++ - "header2.h"
			if (args.IncludedFile.Spelling == "header2.h")
			{
				Assert.IsFalse(args.IncludedFile.IsAngled);
				Assert.IsFalse(args.IncludedFile.IsImport);
				Assert.IsTrue(args.IncludedFile.IsModuleImport);
			}
		}

		/// <summary>
		/// Handler for Index.StartedTranslationUnit
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="args">Event arguments</param>
		private void OnStartedTranslationUnit(object sender, IndexEventArgs args)
		{
			// Sender should always be the Index
			Assert.IsNotNull(sender);
			Assert.IsInstanceOfType(sender, typeof(Index));

			// Ensure that the state object is not null and is a List<string>
			Assert.IsNotNull(args);
			Assert.IsNotNull(args.State);
			Assert.AreSame(args.State, args.State);
			Assert.IsInstanceOfType(args.State, typeof(IndexerState));
		}

		[TestMethod(), TestCategory("Indexer")]
		public void Indexer_IndexSourceFile()
		{
			// The indexer doesn't work with threads under the managed debugger
			Assert.IsTrue(Environment.GetEnvironmentVariables().Contains("LIBCLANG_NOTHREADS"));

			// Enable module mapping for the indexer/translation unit
			string[] args = new string[] { "-fmodules" };

			// Handle the C++ declarations and entities with this test
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\indexer.cpp");
			using (Index index = Clang.CreateIndex())
			{
				// Add all the handlers
				index.Abort += OnAbort;
				index.Declaration += OnDeclaration;
				index.Diagnostics += OnDiagnostics;
				index.EnteredMainFile += OnEnteredMainFile;
				index.EntityReference += OnEntityReference;
				index.ImportedASTFile += OnImportedASTFile;
				index.IncludedFile += OnIncludedFile;
				index.StartedTranslationUnit += OnStartedTranslationUnit;

				IndexerState state = new IndexerState();
				index.IndexSourceFile(inpath, args, IndexOptions.None, state);
			}
		}

		[TestMethod(), TestCategory("Indexer")]
		public void Indexer_IndexSourceFile_FullArgv()
		{
			// The indexer doesn't work with threads under the managed debugger
			Assert.IsTrue(Environment.GetEnvironmentVariables().Contains("LIBCLANG_NOTHREADS"));

			using (Index index = Clang.CreateIndex())
			{
				// Make sure that the ArgumentsAreFullCommandLine option throws if no arguments are specified
				try { index.IndexSourceFile("hello.cpp", IndexOptions.ArgumentsAreFullCommandLine); Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ArgumentNullException)); }

				// I'm not entirely sure what value this option really holds since it doesn't do what I assumed it
				// did, so for now just make sure it doesn't blow up in the caller's face
				string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
				string[] args = new string[] { Path.Combine(Path.GetDirectoryName(inpath), "clang") };
				index.IndexSourceFile(inpath, args, IndexOptions.ArgumentsAreFullCommandLine);
			}
		}

		[TestMethod(), TestCategory("Indexer")]
		public void Indexer_IndexTranslationUnit()
		{
			// The indexer doesn't work with threads under the managed debugger
			Assert.IsTrue(Environment.GetEnvironmentVariables().Contains("LIBCLANG_NOTHREADS"));

			// Enable module mapping for the indexer/translation unit
			string[] args = new string[] { "-fmodules" };

			// Handle the objective-c declarations and entities with this test
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\indexer.m");
			using (TranslationUnit transunit = Clang.CreateTranslationUnit(inpath, args, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				using (Index index = Clang.CreateIndex())
				{
					// Add all the handlers
					index.Abort += OnAbort;
					index.Declaration += OnDeclaration;
					index.Diagnostics += OnDiagnostics;
					index.EnteredMainFile += OnEnteredMainFile;
					index.EntityReference += OnEntityReference;
					index.ImportedASTFile += OnImportedASTFile;
					index.IncludedFile += OnIncludedFile;
					index.StartedTranslationUnit += OnStartedTranslationUnit;

					IndexerState state = new IndexerState();
					index.IndexTranslationUnit(transunit, IndexOptions.None, state);
				}
			}
		}

		[TestMethod(), TestCategory("Indexer")]
		public void IndexEntityKind_Equality()
		{
			IndexEntityKind kind = IndexEntityKind.CxxClass;
			Assert.IsTrue(kind == IndexEntityKind.CxxClass);
			Assert.IsTrue(IndexEntityKind.CxxClass == kind);
			Assert.AreEqual(kind, IndexEntityKind.CxxClass);
			Assert.AreEqual(IndexEntityKind.CxxClass, kind);
			Assert.IsTrue(kind.Equals(IndexEntityKind.CxxClass));
			Assert.IsTrue(IndexEntityKind.CxxClass.Equals(kind));
		}

		[TestMethod(), TestCategory("Indexer")]
		public void IndexEntityKind_Inequality()
		{
			IndexEntityKind kind = IndexEntityKind.CxxClass;
			Assert.IsTrue(kind != IndexEntityKind.Field);
			Assert.IsTrue(IndexEntityKind.Field != kind);
			Assert.AreNotEqual(kind, IndexEntityKind.Field);
			Assert.AreNotEqual(IndexEntityKind.Field, kind);
			Assert.IsFalse(kind.Equals(IndexEntityKind.Field));
			Assert.IsFalse(IndexEntityKind.Field.Equals(kind));
		}

		[TestMethod(), TestCategory("Indexer")]
		public void IndexEntityKind_GetHashCode()
		{
			// Just pick a few
			Assert.AreNotEqual(0, IndexEntityKind.Typedef.GetHashCode());
			Assert.AreNotEqual(0, IndexEntityKind.EnumConstant.GetHashCode());
			Assert.AreNotEqual(0, IndexEntityKind.CxxClass.GetHashCode());
		}

		[TestMethod(), TestCategory("Indexer")]
		public void IndexEntityKind_IsObjectiveCContainer()
		{
			// Just pick a few
			Assert.IsTrue(IndexEntityKind.ObjectiveCCategory.IsObjectiveCContainer);
			Assert.IsTrue(IndexEntityKind.ObjectiveCProtocol.IsObjectiveCContainer);
			Assert.IsFalse(IndexEntityKind.CxxClass.IsObjectiveCContainer);
		}

		[TestMethod(), TestCategory("Indexer")]
		public void IndexEntityKind_ToString()
		{
			Assert.AreEqual("Unexposed", IndexEntityKind.Unexposed.ToString());
			Assert.AreEqual("Typedef", IndexEntityKind.Typedef.ToString());
			Assert.AreEqual("Function", IndexEntityKind.Function.ToString());
			Assert.AreEqual("Variable", IndexEntityKind.Variable.ToString());
			Assert.AreEqual("Field", IndexEntityKind.Field.ToString());
			Assert.AreEqual("EnumConstant", IndexEntityKind.EnumConstant.ToString());
			Assert.AreEqual("ObjectiveCClass", IndexEntityKind.ObjectiveCClass.ToString());
			Assert.AreEqual("ObjectiveCProtocol", IndexEntityKind.ObjectiveCProtocol.ToString());
			Assert.AreEqual("ObjectiveCCategory", IndexEntityKind.ObjectiveCCategory.ToString());
			Assert.AreEqual("ObjectiveCInstanceMethod", IndexEntityKind.ObjectiveCInstanceMethod.ToString());
			Assert.AreEqual("ObjectiveCClassMethod", IndexEntityKind.ObjectiveCClassMethod.ToString());
			Assert.AreEqual("ObjectiveCProperty", IndexEntityKind.ObjectiveCProperty.ToString());
			Assert.AreEqual("ObjectiveCIvar", IndexEntityKind.ObjectiveCIvar.ToString());
			Assert.AreEqual("Enum", IndexEntityKind.Enum.ToString());
			Assert.AreEqual("Struct", IndexEntityKind.Struct.ToString());
			Assert.AreEqual("Union", IndexEntityKind.Union.ToString());
			Assert.AreEqual("CxxClass", IndexEntityKind.CxxClass.ToString());
			Assert.AreEqual("CxxNamespace", IndexEntityKind.CxxNamespace.ToString());
			Assert.AreEqual("CxxNamespaceAlias", IndexEntityKind.CxxNamespaceAlias.ToString());
			Assert.AreEqual("CxxStaticVariable", IndexEntityKind.CxxStaticVariable.ToString());
			Assert.AreEqual("CxxStaticMethod", IndexEntityKind.CxxStaticMethod.ToString());
			Assert.AreEqual("CxxInstanceMethod", IndexEntityKind.CxxInstanceMethod.ToString());
			Assert.AreEqual("CxxConstructor", IndexEntityKind.CxxConstructor.ToString());
			Assert.AreEqual("CxxDestructor", IndexEntityKind.CxxDestructor.ToString());
			Assert.AreEqual("CxxConversionFunction", IndexEntityKind.CxxConversionFunction.ToString());
			Assert.AreEqual("CxxTypeAlias", IndexEntityKind.CxxTypeAlias.ToString());
			Assert.AreEqual("CxxInterface", IndexEntityKind.CxxInterface.ToString());
		}
	}
}
