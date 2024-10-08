#import <Foundation/Foundation.h>
#import <React/RCTUIManager.h>
#import <reanimated/LayoutAnimations/LayoutAnimationType.h>
#import <reanimated/apple/LayoutReanimation/REASnapshot.h>
#import <reanimated/apple/REANodesManager.h>
#import <reanimated/apple/REAUIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, ViewState) {
  Inactive,
  Appearing,
  Disappearing,
  Layout,
  ToRemove,
};

typedef BOOL (^REAHasAnimationBlock)(NSNumber *_Nonnull tag, LayoutAnimationType type);
typedef BOOL (^REAShouldAnimateExitingBlock)(NSNumber *_Nonnull tag, BOOL shouldAnimate);
typedef void (
    ^REAAnimationStartingBlock)(NSNumber *_Nonnull tag, LayoutAnimationType type, NSDictionary *_Nonnull yogaValues);
typedef void (^REAAnimationRemovingBlock)(NSNumber *_Nonnull tag);
typedef void (^REASharedTransitionRemovingBlock)(NSNumber *_Nonnull tag);
#ifndef NDEBUG
typedef void (^REACheckDuplicateSharedTagBlock)(REAUIView *view, NSNumber *_Nonnull viewTag);
#endif
typedef void (^REACancelAnimationBlock)(NSNumber *_Nonnull tag);
typedef NSNumber *_Nullable (^REAFindPrecedingViewTagForTransitionBlock)(NSNumber *_Nonnull tag);
typedef NSArray<NSNumber *> *_Nullable (^REAGetSharedGroupBlock)(NSNumber *_Nonnull tag);
typedef int (^REATreeVisitor)(id<RCTComponent>);
BOOL REANodeFind(id<RCTComponent> view, int (^block)(id<RCTComponent>));

@interface REAAnimationsManager : NSObject

- (instancetype)initWithUIManager:(RCTUIManager *)uiManager;
- (void)setAnimationStartingBlock:(REAAnimationStartingBlock)startAnimation;
- (void)setHasAnimationBlock:(REAHasAnimationBlock)hasAnimation;
- (void)setShouldAnimateExitingBlock:(REAShouldAnimateExitingBlock)shouldAnimateExiting;
- (void)setAnimationRemovingBlock:(REAAnimationRemovingBlock)clearAnimation;
- (void)setSharedTransitionRemovingBlock:(REASharedTransitionRemovingBlock)clearSharedTransition;
#ifndef NDEBUG
- (void)setCheckDuplicateSharedTagBlock:(REACheckDuplicateSharedTagBlock)checkDuplicateSharedTag;
#endif
- (void)progressLayoutAnimationWithStyle:(NSDictionary *_Nonnull)newStyle
                                  forTag:(NSNumber *_Nonnull)tag
                      isSharedTransition:(BOOL)isSharedTransition;
- (void)setFindPrecedingViewTagForTransitionBlock:
    (REAFindPrecedingViewTagForTransitionBlock)findPrecedingViewTagForTransition;
- (void)setGetSharedGroupBlock:(REAGetSharedGroupBlock)getSharedGroupBlock;
- (void)setCancelAnimationBlock:(REACancelAnimationBlock)animationCancellingBlock;
- (void)endLayoutAnimationForTag:(NSNumber *_Nonnull)tag removeView:(BOOL)removeView;
- (void)endAnimationsRecursive:(REAUIView *)view;
- (void)invalidate;
- (void)viewDidMount:(REAUIView *)view withBeforeSnapshot:(REASnapshot *)snapshot withNewFrame:(CGRect)frame;
- (REASnapshot *)prepareSnapshotBeforeMountForView:(REAUIView *)view;
- (void)removeAnimationsFromSubtree:(REAUIView *)view;
- (void)reattachAnimatedChildren:(NSArray<id<RCTComponent>> *)children
                     toContainer:(id<RCTComponent>)container
                       atIndices:(NSArray<NSNumber *> *)indices;
- (void)onViewCreate:(REAUIView *)view after:(REASnapshot *)after;
- (void)onViewUpdate:(REAUIView *)view before:(REASnapshot *)before after:(REASnapshot *)after;
- (void)viewsDidLayout;
- (NSMutableDictionary *)prepareDataForLayoutAnimatingWorklet:(NSMutableDictionary *)currentValues
                                                 targetValues:(NSMutableDictionary *)targetValues;
- (REAUIView *)viewForTag:(NSNumber *)tag;
- (BOOL)hasAnimationForTag:(NSNumber *)tag type:(LayoutAnimationType)type;
- (void)clearAnimationConfigForTag:(NSNumber *)tag;
- (void)clearSharedTransitionConfigForTag:(NSNumber *)tag;
- (void)startAnimationForTag:(NSNumber *)tag type:(LayoutAnimationType)type yogaValues:(NSDictionary *)yogaValues;
- (void)onScreenRemoval:(REAUIView *)screen stack:(REAUIView *)stack;

@end

NS_ASSUME_NONNULL_END
