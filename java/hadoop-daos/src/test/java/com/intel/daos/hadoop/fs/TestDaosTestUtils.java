package com.intel.daos.hadoop.fs;

import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.conf.Configuration;
import org.junit.internal.AssumptionViolatedException;

import java.io.IOException;
import java.net.URI;

/**
 *
 */
public class TestDaosTestUtils {
  private static Configuration configuration;
  public static final String TEST_FS_DAOS_NAME = "test.fs.daos.name";

  private TestDaosTestUtils(){

  }

  public static DaosFileSystem createTestFileSystem(Configuration conf){
    DaosFileSystem daosFileSystem = new DaosFileSystem();
    try {
      configuration =conf;
      daosFileSystem.initialize(getURI(configuration), configuration);
    } catch (IOException e) {
      e.printStackTrace();
    }
    return daosFileSystem;
  }

  public static Configuration getConfiguration(){
    return configuration!=null ? configuration:null;
  }

  private static URI getURI(Configuration conf) {
    String fsname = conf.getTrimmed(
            TestDaosTestUtils.TEST_FS_DAOS_NAME, "daos:///");

    boolean liveTest = !StringUtils.isEmpty(fsname);
    URI testURI = null;
    if (liveTest) {
      testURI = URI.create(fsname);
      liveTest = testURI.getScheme().equals(Constants.FS_DAOS);
    }

    if (!liveTest) {
      throw new AssumptionViolatedException("No test filesystem in "
          + TestDaosTestUtils.TEST_FS_DAOS_NAME);
    }
    return testURI;
  }

  /**
   * Generate unique test path for multiple user tests.
   *
   * @return root test path
   */
  public static String generateUniqueTestPath() {
    String testUniqueForkId = System.getProperty("test.unique.fork.id");
    return testUniqueForkId == null ? "/test" :
        "/" + testUniqueForkId + "/test";
  }

}
