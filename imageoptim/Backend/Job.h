//
//  File.h
//
//  Created by porneL on 8.wrz.07.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import "Workers/Worker.h"

@class ResultsDb, File;


NS_ASSUME_NONNULL_BEGIN
@interface Job : NSObject <NSCopying, QLPreviewItem> {
	NSURL *filePath, *revertPath;
	NSString *displayName;

    /** size of file before any optimizations */
	NSUInteger byteSizeOriginal;
    /** expected current size of file on disk, updated before and after optimization */
    NSUInteger byteSizeOnDisk;
    /** current best estimate of what optimized file size will be */
    NSUInteger byteSizeOptimized;

    NSString *bestToolName;
    NSMutableDictionary *bestTools;
	double percentDone;

    NSMutableSet *filePathsOptimizedInUse;

	NSString *statusImageName;
    NSString *statusText;
    NSInteger statusOrder;

    NSMutableArray *workers;
    NSMutableDictionary *workersPreviousResults;

    NSOperationQueue *fileIOQueue;
    ResultsDb *db;
    uint32_t settingsHash[4];
    uint32_t inputFileHash[4];

    BOOL done, failed, optimized, stopping, lossyConverted;
}

-(BOOL)isBusy;
-(BOOL)isStoppable;
-(BOOL)isOptimized;

-(BOOL)stop;
-(BOOL)revert;
@property (readonly) BOOL canRevert;
@property (readonly) BOOL isDone, isFailed;

-(void)enqueueWorkersInCPUQueue:(NSOperationQueue *)queue fileIOQueue:(NSOperationQueue *)fileIOQueue defaults:(NSUserDefaults*)defaults;

-(BOOL)setFileOptimized:(nullable File *)f toolName:(NSString *)s;

-(nullable instancetype)initWithFilePath:(NSURL *)aPath resultsDatabase:(nullable ResultsDb *)aDb;
-(id)copyWithZone:(nullable NSZone *)zone;
-(void)resetToOriginalByteSize:(NSUInteger)size;
-(void)setByteSizeOptimized:(NSUInteger)size;
-(void)updateStatusOfWorker:(nullable Worker *)currentWorker running:(BOOL)started;

-(void)setFilePath:(NSURL *)s;

@property (readonly, nullable) File *initialInput, *unoptimizedInput, *wipInput, *savedOutput;
@property (readonly, copy) NSString *fileName;

@property (strong, nullable) NSString *statusText, *bestToolName;
@property (strong) NSString *displayName;
@property (strong, nonatomic) NSURL *filePath;
@property (strong) NSString *statusImageName;
@property (assign,nonatomic) NSUInteger byteSizeOriginal, byteSizeOptimized;
@property (assign,readonly) NSInteger statusOrder;
@property (strong,readonly) NSMutableDictionary *workersPreviousResults;

@property (assign) double percentDone;

-(void)setStatus:(NSString *)name order:(NSInteger)order text:(NSString *)text;
-(void)setError:(NSString *)text;
-(void)cleanup;

-(void)doEnqueueWorkersInCPUQueue:(NSOperationQueue *)queue defaults:(NSUserDefaults*)defaults;
@end
NS_ASSUME_NONNULL_END
