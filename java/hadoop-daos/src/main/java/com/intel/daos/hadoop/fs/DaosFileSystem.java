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

import com.google.common.collect.Lists;
import com.intel.daos.client.DaosFile;
import com.intel.daos.client.DaosFsClient;
import com.intel.daos.client.DaosObjectType;
import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.*;
import org.apache.hadoop.fs.permission.FsPermission;
import org.apache.hadoop.util.Progressable;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URI;
import java.util.List;

/**
 * Implementation of {@link FileSystem} for DAOS file system.
 */
public class DaosFileSystem extends FileSystem {
  private static final Logger LOG = LoggerFactory.getLogger(DaosFileSystem.class);
  private Path workingDir;
  private URI uri;
  private DaosFsClient daos = null;
  private int readBufferSize;
  private boolean bufferedReadEnabled;
  private int writeBufferSize;
  private int blockSize;
  private int chunksize;

  @Override
  public void initialize(URI name, Configuration conf)
          throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem initialize");
    }
    if (!getScheme().equals(name.getScheme())){
      throw new IllegalArgumentException("schema should be "+getScheme());
    }

    super.initialize(name, conf);

    try {
      // get conf from hdfs-site.xml
      this.readBufferSize = conf.getInt(Constants.DAOS_READ_BUFFER_SIZE, Constants.DEFAULT_DAOS_READ_BUFFER_SIZE);
      this.writeBufferSize = conf.getInt(Constants.DAOS_WRITE_BUFFER_SIZE, Constants.DEFAULT_DAOS_WRITE_BUFFER_SIZE);
      this.blockSize = conf.getInt(Constants.DAOS_BLOCK_SIZE, Constants.DEFAULT_DAOS_BLOCK_SIZE);
      this.chunksize = conf.getInt(Constants.DAOS_CHUNK_SIZE, Constants.DEFAULT_DAOS_CHUNK_SIZE);
      bufferedReadEnabled = conf.getBoolean(Constants.BUFFERED_READ_ENABLED, Constants.DEFAULT_BUFFERED_READ_ENABLED);

      checkSizeMin(readBufferSize, Constants.DEFAULT_DAOS_READ_BUFFER_SIZE, "internal read buffer size should be no less than ");
      checkSizeMin(writeBufferSize, Constants.DEFAULT_DAOS_WRITE_BUFFER_SIZE, "internal write buffer size should be no less than ");
      checkSizeMin(blockSize, Constants.MINIMUM_DAOS_BLOCK_SIZE, "block size should be no less than ");
      checkSizeMin(chunksize, Constants.DEFAULT_DAOS_CHUNK_SIZE, "daos chunk size should be no less than ");

      checkSizeMax(readBufferSize, Constants.MAXIMUM_DAOS_READ_BUFFER_SIZE, "internal read buffer size should not be greater than ");
      checkSizeMax(writeBufferSize, Constants.MAXIMUM_DAOS_WRITE_BUFFER_SIZE, "internal write buffer size should not be greater than ");
      checkSizeMax(blockSize, Constants.MAXIMUM_DAOS_BLOCK_SIZE, "block size should be not be greater than ");
      checkSizeMax(chunksize, Constants.MAXIMUM_DAOS_CHUNK_SIZE, "daos chunk size should not be greater than ");

      String poolUuid = conf.get(Constants.DAOS_POOL_UUID);
      if (StringUtils.isEmpty(poolUuid)) {
        throw new IOException(Constants.DAOS_POOL_UUID + " is null , need to set up " + Constants.DAOS_POOL_UUID +" in hdfs.xml .");
      }
      String contUuid = conf.get(Constants.DAOS_CONTAINER_UUID);
      if (StringUtils.isEmpty(contUuid)) {
        throw new IOException(Constants.DAOS_CONTAINER_UUID + " is null, need to set up " + Constants.DAOS_CONTAINER_UUID +" in hdfs.xml .");
      }
      String svc = conf.get(Constants.DAOS_POOL_SVC);
      if (StringUtils.isEmpty(svc)) {
        throw new IOException(Constants.DAOS_POOL_SVC + " is null, need to set up " + Constants.DAOS_POOL_SVC + " in hdfs.xml .");
      }

      // daosFSclient build
      //TODO: uncomment
      this.daos = new DaosFsClient.DaosFsClientBuilder().poolId(poolUuid).containerId(contUuid).ranks(svc).build();
      this.uri = URI.create(name.getScheme() + "://" + name.getAuthority());
      this.workingDir = new Path("/user", System.getProperty("user.name")).
              makeQualified(this.uri, this.getWorkingDirectory());
      //TODO: mkdir workingDir in DAOS
      setConf(conf);
    } catch (IOException e) {
      throw new IOException("failed to initialize "+this.getClass().getName(), e);
    }
  }

  private void checkSizeMin(int size, int min, String msg){
    if (size < min) {
      throw new IllegalArgumentException(msg + min);
    }
  }

  private void checkSizeMax(int size, int max, String msg){
    if (size > max) {
      throw new IllegalArgumentException(msg + max);
    }
  }

  /**
   * Return the protocol scheme for the FileSystem.
   *
   * @return {@link Constants#DAOS_SCHEMA}
   */
  @Override
  public String getScheme() {
    return Constants.DAOS_SCHEMA;
  }

  @Override
  public URI getUri() {
    return uri;
  }

  @Override
  public FSDataInputStream open(
          Path f,
          final int bufferSize) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem open :  path = " + f.toUri().getPath() +" ; buffer size = " + bufferSize);
    }

    DaosFile file = daos.getFile(f.toUri().getPath());
    if (!file.exists()) {
      throw new FileNotFoundException(f + "not exist");
    }

    return new FSDataInputStream(new DaosInputStream(file, statistics, readBufferSize, bufferedReadEnabled));
  }

  @Override
  public FSDataOutputStream create(Path f,
                                   FsPermission permission,
                                   boolean overwrite,
                                   int bufferSize,
                                   short replication,
                                   long bs,
                                   Progressable progress)throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem create file , path= " + f.toUri().toString()
              + ", buffer size = " + bufferSize
              + ", block size = " + bs);
    }
    String key = f.toUri().getPath();

    DaosFile daosFile = this.daos.getFile(key);

    if (daosFile.exists()) {
      throw new FileAlreadyExistsException(f + " already exists");
    }

    daosFile.createNewFile(
              Constants.DAOS_MODLE,
              DaosObjectType.OC_SX,
              this.chunksize);

    return new FSDataOutputStream(new DaosOutputStream(daosFile, key, writeBufferSize), statistics);
  }

  @Override
  public FSDataOutputStream append(Path f,
                                   int bufferSize,
                                   Progressable progress)throws IOException {
    throw new IOException("Append is not supported");
  }

  @Override
  public boolean rename(Path src, Path dst) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem: rename old path {} to new path {}" ,src.toUri().getPath(), dst.toUri().getPath());
    }
    String srcPath = src.toUri().getPath();
    String destPath = dst.toUri().getPath();
    // determine  if src is root dir and whether it exits
    if(src.toUri().getPath().equals("/")){
      if(LOG.isDebugEnabled()){
        LOG.debug("DaosFileSystem:  cat not rename root path %s",src);
      }
      return false;
    }

    if (srcPath.equals(destPath)){
      return false;
    }

    //TODO: check error code for specific exception
    //FileAlreadyExistsException
    //FileNotFoundException
    try {
      daos.move(srcPath, destPath);
      return true;
    }catch (IOException e){
      return false;
    }
  }

  @Override
  public boolean delete(Path f, boolean recursive) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem:   delete  path = {} - recursive = {}", f.toUri().getPath(), recursive);
    }
    try {
      daos.delete(f.toUri().getPath(), recursive);
      return true;
    }catch (IOException e) {////TODO: check error code for specific exception
      return false;
    }
  }

  @Override
  public FileStatus[] listStatus(Path f) throws IOException{
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem listStatus :  List status for path = %s " ,f.toUri().getPath());
    }

    final List<FileStatus> result = Lists.newArrayList();
    try {
      String key = f.toUri().getPath();

      final FileStatus fileStatus = getFileStatus(f);
      if(fileStatus==null) {
        return result.toArray(new FileStatus[result.size()]);
      }
      if (fileStatus.isDirectory()) {
        if (LOG.isDebugEnabled()) {
          LOG.debug("DaosFileSystem listStatus : doing listFile for directory %s " , f.toUri().getPath());
        }
        String[] fileString = daos.getFile(key).listChildren();
        if(fileString==null||fileString.length <= 0 ){
          return result.toArray(new FileStatus[result.size()]);
        }
        for (String filePath : fileString) {
          if(filePath.equals("/")){
            FileStatus file =getFileStatus(new Path(key + filePath).makeQualified(this.uri, this.workingDir));
            result.add(file);
          }else{
            FileStatus file = getFileStatus(new Path(key + "/" + filePath).makeQualified(this.uri, this.workingDir));
            result.add(file);
          }
        }
      } else {
        result.add(fileStatus);
      }
    } catch (FileNotFoundException e) {
      throw new FileNotFoundException(e.getMessage());
    }
    return result.toArray(new FileStatus[result.size()]);
  }

  @Override
  public void setWorkingDirectory(Path newdir) {
    workingDir = newdir;
  }

  @Override
  public Path getWorkingDirectory() {
    return workingDir;
  }

  @Override
  public boolean mkdirs(Path f, FsPermission permission) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem mkdirs: Making directory = %s ",f.toUri().getPath());
    }
    String key = f.toUri().getPath();
    try{
      FileStatus status = getFileStatus(f);
      // if the thread reaches here, there is something at the path
      if (status.isDirectory()) {
        return true;
      }else{
        throw new FileAlreadyExistsException("Not a directory: " + f);
      }
    }catch(FileNotFoundException e){
      validatePath(f);
      daos.getFile(key).mkdirs();
      return true;
    }
  }

  /**
   * Check whether the path is a valid path.
   *
   * @param path the path to be checked.
   * @throws IOException
   */
  private void validatePath(Path path) throws IOException {
    Path fPart = path.getParent();
    do {
      try {
        FileStatus fileStatus = getFileStatus(fPart);
        if (fileStatus.isDirectory()) {
          // If path exists and a directory, exit
          break;
        } else {
          throw new FileAlreadyExistsException(String.format(
                  "Can't make directory for path '%s', it is a file.", fPart));
        }
      } catch (FileNotFoundException fnfe) {
      }
      fPart = fPart.getParent();
    } while (fPart != null);
  }

  @Override
  public FileStatus getFileStatus(Path f) throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem getFileStatus:  Get File Status , path= %s ",f.toUri().getPath());
    }
    String key = f.toUri().getPath();
    DaosFile daosFile = daos.getFile(key);
    if (!daosFile.exists()) {
      throw new FileNotFoundException("File does not exist: " + key);
    }
    //TODO: get statAttributes
    if (daosFile.isDirectory()) {
      return new FileStatus(0, true, 1, this.blockSize, 0, f);
    }

    long length = daosFile.length();
    return new FileStatus(length, false, 1, this.blockSize, 0, f);
  }

  @Override
  public boolean exists(Path f){
    if (LOG.isDebugEnabled()) {
      LOG.debug(" DaosFileSystem exists: Is path = %s exists",f.toUri().getPath());
    }
    try {
      String key = f.toUri().getPath();
      return daos.getFile(key).exists();
    } catch (IOException e) {
      return false;
    }
  }

  @Override
  public void close() throws IOException {
    if (LOG.isDebugEnabled()) {
      LOG.debug("DaosFileSystem close");
    }
    super.close();
  }

//  DaosFile getFile(Path path){
//    weakCache.get(path);
//  }
}
