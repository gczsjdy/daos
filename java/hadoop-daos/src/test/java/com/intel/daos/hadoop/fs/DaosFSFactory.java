package com.intel.daos.hadoop.fs;

import com.intel.daos.client.DaosFile;
import com.intel.daos.client.DaosFsClient;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;

import java.io.IOException;

/**
 *
 */
public class DaosFSFactory {
  private final static String pooluuid = "0eba76a4-5f9d-4c47-91c7-545b3677fb28";
  private final static String contuuid = "676074c6-a33a-4e07-8990-fe9279065145";
  private final static String svc = "0";

  private static FileSystem createFS()throws IOException {
    Configuration conf = new Configuration();
    conf.set(Constants.DAOS_POOL_UUID, pooluuid);
    conf.set(Constants.DAOS_CONTAINER_UUID, contuuid);
    conf.set(Constants.DAOS_POOL_SVC, svc);
    return DaosUtils.createTestFileSystem(conf);
  }

  public synchronized static FileSystem getFS() throws IOException{
    prepareFs();
    return createFS();
  }

  public synchronized static DaosFsClient getFsClient() throws IOException{
    DaosFsClient.DaosFsClientBuilder builder = new DaosFsClient.DaosFsClientBuilder();
    builder.poolId(pooluuid).containerId(contuuid);
    return builder.build();
  }

  private static DaosFsClient prepareFs()throws IOException{
    try {
      DaosFsClient client = getFsClient();
      //clear all content
      DaosFile daosFile = client.getFile("/");
      String[] children = daosFile.listChildren();
      for(String child : children) {
        if(child.length() == 0 || ".".equals(child)){
          continue;
        }
        String path = "/"+child;
        DaosFile childFile = client.getFile(path);
        if(childFile.delete(true)){
          System.out.println("deleted folder "+path);
        }else{
          System.out.println("failed to delete folder "+path);
        }
      }
      return client;
    }catch (Exception e){
      System.out.println("failed to clear/prepare file system");
      e.printStackTrace();
    }
    return null;
  }
}
