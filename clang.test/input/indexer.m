@class Foo;
@interface Bar
@property (retain) __attribute__((iboutletcollection(Foo))) Foo *prop;
@end

@interface I
-(id)prop __attribute__((annotate("anno")));
-(void)setProp:(id)p __attribute__((annotate("anno")));
@property (assign) id prop __attribute__((annotate("anno")));
@end

@interface I2
@property (readonly) id prop;
 -(id)prop;
@end

@interface I2()
@property (assign,readwrite) id prop;
@end

@implementation I2
@synthesize prop = _prop;
@end

int test1() {
  extern int extvar;
  extvar = 2;
  extern int extfn();
  return extfn();
}

@interface I4
@property (assign, nonatomic) id prop;
-(id)prop;
-(void)setProp:(id)p;
@end

@implementation I4
@synthesize prop = _prop;
-(id)prop {
  return 0;
}
-(void)setProp:(id)p {
}
@end

@interface I6
@end

@interface B6
@end

@protocol P6
@end

@class I6;

@interface I6(cat)
-(I6*)meth;
@end

@class I6;

@interface S6 : B6<P6>
-(void)meth:(B6*)b :(id<P6>)p;
@end