package com.intel.daos.hadoop.fs;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;

/**
 *
 */
public class DaosFSFactory {
  private static FileSystem fs = null;
  private final static String pooluuid = "53a47469-ea2a-418e-89d3-6d1df1aaadb4";
  private final static String contuuid = "9e60aff2-ca28-45fe-bdb0-d1a6c182c342";
  private final static String svc = "0";

  private static void createFS(){
    Configuration conf = new Configuration();
    conf.set(Constants.DAOS_POOL_UUID, pooluuid);
    conf.set(Constants.DAOS_CONTAINER_UUID, contuuid);
    conf.set(Constants.DAOS_POOL_SVC, svc);
    fs = TestDaosTestUtils.createTestFileSystem(conf);
  }

  public static FileSystem getFS() {
    if(fs == null){
      createFS();
    }
    return fs;
  }

  public static String getPoolUuid() {
    return pooluuid;
  }

  public static String getContUuid() {
    return contuuid;
  }
}
