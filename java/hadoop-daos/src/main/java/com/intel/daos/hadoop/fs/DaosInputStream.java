/*
 * (C) Copyright 2018-2019 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * GOVERNMENT LICENSE RIGHTS-OPEN SOURCE SOFTWARE
 * The Government's rights to use, modify, reproduce, release, perform, display,
 * or disclose this software are subject to the terms of the Apache License as
 * provided in Contract No. B609815.
 * Any reproduction of computer software, computer software documentation, or
 * portions thereof marked with this legend must also reproduce the markings.
 */

package com.intel.daos.hadoop.fs;

import com.intel.daos.client.DaosFile;
import org.apache.hadoop.fs.FSInputStream;
import org.apache.hadoop.fs.FileSystem;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.EOFException;
import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * The input stream for Daos system.
 */
public class DaosInputStream extends FSInputStream {

  private final DaosFile daosFile;
  private final FileSystem.Statistics stats;
  private final long fileLen;
  private final int bufferCapacity;
  private final int preLoadSize;

  private ByteBuffer buffer;
  private int bufEnd;   // buffer offset from which data ends

  private byte[] singleByte = new byte[]{0};

  private long lastFilePos;  // file offset from which file data is read to buffer
  private long nextReadPos;  // next read position

  private boolean closed;

  private static final Logger LOG = LoggerFactory.getLogger(DaosInputStream.class);

  public DaosInputStream(DaosFile daosFile,
                         FileSystem.Statistics stats,
                         int bufferSize, int preLoadSize) throws IOException{
    this.daosFile = daosFile;
    this.stats = stats;
    this.buffer = ByteBuffer.allocateDirect(bufferSize);
    this.fileLen = daosFile.length();
    this.bufferCapacity = bufferSize;
    this.preLoadSize = preLoadSize;
  }

  @Override
  public synchronized void seek(long targetPos) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream : Seek targetPos = " + targetPos
              + "; current position = " + getPos() + "; next read position= " + this.nextReadPos);
    }
    checkNotClose();

    if (targetPos < 0) {
      throw new EOFException("Cannot seek to negative position " + targetPos);
    }
    if (this.fileLen < targetPos) {
      throw new EOFException("Cannot seek after EOF ,file length :" + fileLen +" ; targetPos: " + targetPos);
    }

    this.nextReadPos = targetPos;
  }

  @Override
  public synchronized long skip(long len) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream : skip specify length : {}", len);
    }
    if (len > 0){
      long curPos = getPos();
      if(len+curPos > fileLen){
        len = fileLen - curPos;
      }
      seek(curPos+len);
      return len;
    }
    return 0;
  }

  /**
   * @throws IOException
   */
  private synchronized void checkNotClose() throws IOException {
    if (this.closed) {
      throw new IOException("Stream is closed!");
    }
  }

  @Override
  public synchronized long getPos() throws IOException {
    return nextReadPos;
  }

  // Used by unit tests.
  long getFilePos() {
    return this.nextReadPos;
  }

  @Override
  public boolean seekToNewSource(long targetPos) throws IOException {
    checkNotClose();
    return false;
  }

  /**
   * Reads the next byte of data from the input stream.
   */
  @Override
  public synchronized int read() throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream :Reads the next byte of data from the input stream");
    }
    checkNotClose();
    int actualLen = read(singleByte, 0, 1);
    return actualLen<=0 ? -1:this.singleByte[0] & 0xff;
  }

  /**
   *
   */
  @Override
  public synchronized int read(byte[] buf, int off, int len)
      throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream : read from daos , contentLength = " + this.fileLen + " ;  currentPos = " +
              getPos() + "; filePos = " +this.nextReadPos);
    }
    checkNotClose();

    if (off < 0 || len < 0) {
      throw new IllegalArgumentException("offset/length is negative , offset = " + off + ", length = " + len);
    }
    if (len > buf.length - off){
      throw new IndexOutOfBoundsException("requested more bytes than destination buffer size "
              +" : request length = " + len + ", with offset = " + off + ", buffer capacity =" + (buf.length - off ));
    }
    if (len == 0) {
      return 0;
    }

    // check buffer overlay
    long start = lastFilePos;
    long end = lastFilePos + bufEnd;
    if (nextReadPos >= start && nextReadPos < end){ // some requested data in buffer
      long remaining = end - nextReadPos;
      if (remaining >= len) {// all requested data in buffer
        buffer.get(buf, off, len);
        nextReadPos += len;
        this.stats.incrementBytesRead(len);
        return len;
      }
      // part of data in buffer
      buffer.get(buf, off, (int)remaining);
      nextReadPos += remaining;
      off += remaining;
      // read more from file
      long moreLen = readFromDaos(buf, off, (int)(len - remaining));
      long actualLen = remaining + moreLen;
      this.stats.incrementBytesRead(actualLen);
      return (int)actualLen;
    }
    // data not in buffer
    long actualLen = readFromDaos(buf, off, len);
    this.stats.incrementBytesRead(actualLen);
    if (actualLen == 0 && nextReadPos == fileLen){
      return -1; // reach end of file
    }
    return (int)actualLen;
  }

  private long readFromDaos(byte[] buf, int off, int len) throws IOException {
    long totalLen = 0;
    boolean next = true;
    while (len > 0 && next) {
      long actualLen = readFromDaos(len);
      if (actualLen == 0){
        continue;
      }
      if (len <= actualLen) {
        buffer.get(buf, off, len);
        totalLen += len;
        nextReadPos += len;
        next = false;
      } else {
        buffer.get(buf, off, (int)actualLen);
        totalLen += actualLen;
        nextReadPos += len;
        off += actualLen;
        len -= actualLen;
      }
    }
    return totalLen;
  }

  private void clearBuffer() {
    buffer.clear();
    bufEnd = 0;
  }

  /**
   * Read data from DAOS and put into cache buffer.
   */
  private long readFromDaos(long length) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream : read from daos ,filePos = {}" ,this.nextReadPos);
    }
    clearBuffer();

    length = Math.min(length, bufferCapacity);
    length = Math.max(length, preLoadSize);

    long currentTime = 0;
    if (LOG.isDebugEnabled()) {
      currentTime = System.currentTimeMillis();
    }

    long actualLen = this.daosFile.read(this.buffer,0, this.nextReadPos, length);
    lastFilePos = nextReadPos;
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream :reading from daos_api spend time is :  "
          + (System.currentTimeMillis()-currentTime)
          + " ; read data size : " + actualLen);
    }
    return actualLen;
  }

  @Override
  public synchronized void close() throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream close : FileSystem.Statistics = {}" ,this.stats.toString());
    }
    if (this.closed) {
      return;
    }
    this.closed = true;
    this.daosFile.release();

    if (this.buffer!=null) {
      ((sun.nio.ch.DirectBuffer)this.buffer).cleaner().clean();
      this.buffer = null;
    }
    super.close();
  }

  @Override
  public synchronized int available() throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosInputStream available");
    }
    checkNotClose();
    long remaining = this.fileLen - this.nextReadPos;
    if (remaining > Integer.MAX_VALUE) {
      return Integer.MAX_VALUE;
    }
    return (int) remaining;
  }
}
