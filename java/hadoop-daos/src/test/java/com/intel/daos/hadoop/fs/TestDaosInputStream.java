package com.intel.daos.hadoop.fs;

import com.intel.daos.client.DaosFile;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.contract.ContractTestUtils;
import org.apache.hadoop.io.IOUtils;
import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.Timeout;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.Random;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Answers.RETURNS_SMART_NULLS;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.when;

/**
 *
 */
public class TestDaosInputStream {
  private static final Logger LOG = LoggerFactory.getLogger(TestDaosInputStream.class);
  private static FileSystem fs;
  private static String testRootPath = TestDaosTestUtils.generateUniqueTestPath();

  @Mock(answer = RETURNS_SMART_NULLS) DaosFile file;

  @Rule
  public Timeout testTimeout = new Timeout(30 * 60 * 1000);

  @Before
  public void setup() throws IOException {
    System.out.println("@BeforeClass");
    fs = DaosFSFactory.getFS();
    fs.mkdirs(new Path(testRootPath));
  }

  @After
  public void tearDown() throws Exception {
    System.out.println("@AfterClass");
    if (fs != null) {
      fs.delete(new Path(testRootPath), true);
    }
    fs.close();
  }

  private Path setPath(String path) throws IOException {
    Path p ;
    if (path.startsWith("/")) {
      p = new Path(testRootPath + path);
    } else {
      p = new Path(testRootPath + "/" + path);
    }
    if(!fs.exists(p.getParent())){
      fs.mkdirs(p.getParent());
    }
    return p;
  }

  @Test
  public void testSeekFile() throws Exception {
    Path smallSeekFile = setPath("/test.txt");
    long size = 5 * 1024 * 1024;

    ContractTestUtils.generateTestFile(fs, smallSeekFile, size, 256, 255);
    LOG.info("5MB file created: smallSeekFile.txt");

    FSDataInputStream instream = fs.open(smallSeekFile);
    int seekTimes = 5;
    LOG.info("multiple fold position seeking test...:");
    for (int i = 0; i < seekTimes; i++) {
      long pos = size / (seekTimes - i) - 1;
      LOG.info("begin seeking for pos: " + pos);
      instream.seek(pos);
      assertTrue("expected position at:" + pos + ", but got:"
          + instream.getPos(), instream.getPos() == pos);
      LOG.info("completed seeking at pos: " + instream.getPos());
    }
    LOG.info("random position seeking test...:");
    Random rand = new Random();
    for (int i = 0; i < seekTimes; i++) {
      long pos = Math.abs(rand.nextLong()) % size;
      LOG.info("begin seeking for pos: " + pos);
      instream.seek(pos);
      assertTrue("expected position at:" + pos + ", but got:"
          + instream.getPos(), instream.getPos() == pos);
      LOG.info("completed seeking at pos: " + instream.getPos());
    }
    IOUtils.closeStream(instream);
  }

  @Test
  public void testReadFile() throws Exception {
    final int bufLen = 256;
    final int sizeFlag = 5;
    String filename = "readTestFile_" + sizeFlag + ".txt";
    Path readTestFile = setPath("/test/" + filename);
    long size = 2 * 1024 * 1024;

    ContractTestUtils.generateTestFile(fs, readTestFile, size, 256, 255);
    LOG.info(sizeFlag + "MB file created: /test/" + filename);

    FSDataInputStream instream = fs.open(readTestFile);
    byte[] buf = new byte[bufLen];
    long bytesRead = 0;
    while (bytesRead < size) {
      int bytes = 0 ;
      if (size - bytesRead < bufLen) {
        int remaining = (int) (size - bytesRead);
        bytes = instream.read(buf, 0, remaining);
      } else {
        bytes = instream.read(buf, 0, bufLen);
      }
      bytesRead += bytes;

      if (bytesRead % (1024 * 1024) == 0) {
        int available = instream.available();
        int remaining = (int) (size - bytesRead);
        assertTrue("expected remaining:" + remaining + ", but got:" + available,
            remaining == available);
        LOG.info("Bytes read: " + Math.round((double) bytesRead / (1024 * 1024))
            + " MB");
      }
    }
    assertTrue(instream.available() == 0);
    IOUtils.closeStream(instream);
  }

  @Test
  public void testSequentialAndRandomRead() throws Exception {
      Path smallSeekFile = setPath("/test/smallSeekFile.txt");
      long size = 5 * 1024 * 1024;

      ContractTestUtils.generateTestFile(this.fs, smallSeekFile, size, 256, 255);
      LOG.info("5MB file created: smallSeekFile.txt");

      FSDataInputStream fsDataInputStream = this.fs.open(smallSeekFile);
      assertTrue("expected position at:" + 0 + ", but got:"
              + fsDataInputStream.getPos(), fsDataInputStream.getPos() == 0);
      DaosInputStream in =
              (DaosInputStream)fsDataInputStream.getWrappedStream();
      byte[] buf = new byte[Constants.DEFAULT_DAOS_READ_BUFFER_SIZE];
      in.read(buf,0, Constants.DEFAULT_DAOS_READ_BUFFER_SIZE);
      assertTrue("expected position at:"
                      + Constants.DEFAULT_DAOS_READ_BUFFER_SIZE + ", but got:"
                      + in.getFilePos(),
              in.getFilePos() == Constants.DEFAULT_DAOS_READ_BUFFER_SIZE);

      fsDataInputStream.seek(4 * 1024 * 1024);
    in.read(buf,0, Constants.DEFAULT_DAOS_READ_BUFFER_SIZE);
    assertTrue("expected position at:" + 4 * 1024 * 1024
                      + Constants.DEFAULT_DAOS_READ_BUFFER_SIZE + ", but got:"
                      + in.getFilePos(),
              in.getFilePos() == 4 * 1024 * 1024
                      + Constants.DEFAULT_DAOS_READ_BUFFER_SIZE);

      IOUtils.closeStream(fsDataInputStream);
  }

  @Test
  public void testBufferedAndNonBufferedRead() throws IOException {
    MockitoAnnotations.initMocks(this);
    FileSystem.Statistics stats = new FileSystem.Statistics("daos:///");

    int bufferSize = 7;
    byte[] data = new byte[]{19, 49, 89, 64, 20, 19, 1, 2, 3};

    doAnswer(
            invocationOnMock -> {
              ByteBuffer buffer = (ByteBuffer) invocationOnMock.getArguments()[0];
              long bufferOffset = (long) invocationOnMock.getArguments()[1];
              long len = (long) invocationOnMock.getArguments()[3];
              for (long i = bufferOffset; i < bufferOffset + len; i++) {
                buffer.put((int) i, data[(int) (i - bufferOffset)]);
              }
              return len;
            })
            .when(file)
            .read(any(ByteBuffer.class), anyLong(), anyLong(), anyLong());

    boolean[] trueTrueFalseFalse = new boolean[]{true, false};

    for (int j = 0; j < 2; j ++) {
      boolean bufferedReadEnabled = trueTrueFalseFalse[j];
      DaosInputStream input = new DaosInputStream(file, stats, bufferSize, bufferedReadEnabled);
      int readSize = 4;
      byte[] answer = new byte[readSize];
      input.read(answer, 0, readSize);
      byte[] expect = new byte[readSize];
      for (int i = 0; i < readSize; i++) {
        expect[i] = data[i];
      }
      assertArrayEquals(expect, answer);

      boolean shouldThemEqual = bufferedReadEnabled;
      for (int i = readSize; i < data.length && i < input.buffer.limit(); i++) {
        // If enabled buffered read, the internal buffer of DataInputStream should be fully filled
        // Otherwise, DaosInputStream's internal buffer is not filled for non-target part
        assert (shouldThemEqual == (input.buffer.get(i) == data[i]));
      }
    }
  }

  @Test
  public void testReadLengthLargerThanBufferSize() throws IOException {
    MockitoAnnotations.initMocks(this);
    FileSystem.Statistics stats = new FileSystem.Statistics("daos:///");

    int bufferSize = 7;
    byte[] data = new byte[]{19, 49, 89, 64, 20, 19, 1, 2, 3};

    doAnswer(
      invocationOnMock -> {
        ByteBuffer buffer = (ByteBuffer) invocationOnMock.getArguments()[0];
        long bufferOffset = (long) invocationOnMock.getArguments()[1];
        long fileOffset = (long) invocationOnMock.getArguments()[2];
        long len = (long) invocationOnMock.getArguments()[3];
        if (len > buffer.capacity() - bufferOffset) {
          throw new IOException(
            String.format("buffer (%d) has no enough space start at %d for reading %d bytes from file",
            buffer.capacity(), bufferOffset, len));
        }
        long actualRead = 0;
        for (long i = bufferOffset; i < bufferOffset + len &&
            (i - bufferOffset + fileOffset) < data.length; i++) {
          buffer.put((int) i, data[(int) (i - bufferOffset + fileOffset)]);
          actualRead ++;
        }
        return actualRead;
      })
      .when(file)
      .read(any(ByteBuffer.class), anyLong(), anyLong(), anyLong());

    boolean[] trueTrueFalseFalse = new boolean[]{true, false};

    for (int j = 0; j < 2; j ++) {
      boolean bufferedReadEnabled = trueTrueFalseFalse[j];
      DaosInputStream input = new DaosInputStream(file, stats, bufferSize, bufferedReadEnabled);
      int readSize = 9;
      byte[] answer = new byte[readSize];
      input.read(answer, 0, readSize);
      byte[] expect = new byte[readSize];
      for (int i = 0; i < readSize; i++) {
        expect[i] = data[i];
      }
      assertArrayEquals(expect, answer);

      boolean shouldThemEqual = bufferedReadEnabled;
      for (int i = readSize; i < data.length && i < input.buffer.limit(); i++) {
        // If enabled buffered read, the internal buffer of DataInputStream should be fully filled
        // Otherwise, DaosInputStream's internal buffer is not filled for non-target part
        assert (shouldThemEqual == (input.buffer.get(i) == data[i]));
      }
    }
  }
}
