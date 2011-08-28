//
//  FilesQueue.h
//
//  Created by porneL on 23.wrz.07.
//

#import <Cocoa/Cocoa.h>
@class File;

@interface FilesQueue : NSObject <NSTableViewDelegate,NSTableViewDataSource> {
	NSTableView *tableView;
	NSArrayController *filesController;
	BOOL isEnabled;
	NSInteger nextInsertRow;
	NSOperationQueue *cpuQueue;
    NSOperationQueue *fileIOQueue;
	NSOperationQueue *dirWorkerQueue;	
	
	NSRecursiveLock *filesControllerLock;
	
    NSHashTable *seenPathHashes;
    
	NSProgressIndicator *progressBar;
    
    NSLock *queueWaitingLock;
    
    NSTask *currentQLManageTask;
}

-(id)initWithTableView:(NSTableView*)a progressBar:(NSProgressIndicator *)p andController:(NSArrayController*)b;
- (NSString *)tableView:(NSTableView *)aTableView toolTipForCell:(NSCell *)aCell rect:(NSRectPointer)rect tableColumn:(NSTableColumn *)aTableColumn row:(int)row mouseLocation:(NSPoint)mouseLocation;
-(void)addPaths:(NSArray *)paths;
-(void)addFilePaths:(NSArray *)paths;
-(void)addPath:(NSString*)s;

-(void) moveObjectsInArrangedObjectsFromIndexes:(NSIndexSet*)indexSet
										toIndex:(NSUInteger)insertIndex;
- (NSUInteger)rowsAboveRow:(NSUInteger)row inIndexSet:(NSIndexSet *)indexSet;
- (NSUInteger)numberOfRowsInTableView:(NSTableView *)tableview;

-(void)runAdded;
-(void)startAgain;

-(IBAction)delete:(id)sender;
-(void)quickLook;

-(void)cleanup;
-(void)setRow:(NSInteger)row;
-(void)openRowInFinder:(NSInteger)row withPreview:(BOOL)preview;

-(NSArray *)fileTypes;
@end
