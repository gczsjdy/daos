package com.intel.daos.hadoop.fs;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.contract.ContractTestUtils;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.Timeout;

import java.io.IOException;

import static org.junit.Assert.assertEquals;

/**
 *
 */
public class TestDaosOutputStream{
  private static FileSystem fs;
  private static String testRootPath =
          TestDaosTestUtils.generateUniqueTestPath();

  @Rule
  public Timeout testTimeout = new Timeout(30 * 60 * 1000);

  @BeforeClass
  public static void setup() throws IOException {
    System.out.println("@BeforeClass");
    fs = DaosFSFactory.getFS();
    fs.mkdirs(new Path(testRootPath));
  }

  @AfterClass
  public  static void tearDown() throws Exception {
    System.out.println("@AfterClass");
    if (fs != null) {
      fs.delete(new Path(testRootPath), true);
    }
    fs.close();
  }

  private Path getTestPath() throws IOException {
    Path p = new Path(testRootPath + "/daos");
    if(!fs.exists(p)){
      fs.mkdirs(p);
    }
    return p;
  }

  @Test
  public void testZeroByteUpload() throws IOException {
    ContractTestUtils.createAndVerifyFile(fs, getTestPath(), 0);
  }

  @Test
  public void testRegularUpload() throws IOException {
    FileSystem.clearStatistics();
    long size = 1024 * 1024;
    FileSystem.Statistics statistics =
            FileSystem.getStatistics("daos", DaosFileSystem.class);
    // This test is a little complicated for statistics, lifecycle is
    // generateTestFile
    //   fs.create(getFileStatus)    read 1
    //   output stream write         write 1
    // path exists(fs.exists)        read 1
    // verifyReceivedData
    //   fs.open(getFileStatus)      read 1
    //   input stream read           read 2(part size is 512K)
    // fs.delete
    //   getFileStatus & delete & exists & create fake dir read 2, write 2
    ContractTestUtils.createAndVerifyFile(fs, getTestPath(), size - 1);
    int readNum = (int) (size-1)/ Constants.DEFAULE_DAOS_READ_BUFFER_SIZE + 1;
    assertEquals(readNum, statistics.getReadOps());
    assertEquals(size - 1, statistics.getBytesRead());
    int writeNum = (int) (size-1)/ Constants.DEFAULT_DAOS_WRITE_BUFFER_SIZE + 1;
    assertEquals(writeNum, statistics.getWriteOps());
    assertEquals(size - 1, statistics.getBytesWritten());
  }

}
