package com.intel.daos.hadoop.fs;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.URI;

/**
 *
 */
public class TestDaosFileSystem {
  private static final Logger LOG = LoggerFactory.getLogger(TestDaosFileSystem.class);

  private static FileSystem fs;

  @BeforeClass
  public static void setup() throws IOException {
    System.out.println("@BeforeClass");
    fs = DaosFSFactory.getFS();
  }

  //every time test one
  @Test
  public void testInitialization() throws Exception{
    initializationTest("daos:///", "daos://null");
    initializationTest("daos://a:b@c", "daos://a:b@c");
    initializationTest("daos://a:b@c/", "daos://a:b@c");
    initializationTest("daos://a:b@c/path", "daos://a:b@c");
    initializationTest("daos://a@c", "daos://a@c");
    initializationTest("daos://a@c/", "daos://a@c");
    initializationTest("daos://a@c/path", "daos://a@c");
    initializationTest("daos://c", "daos://c");
    initializationTest("daos://c/", "daos://c");
    initializationTest("daos://c/path", "daos://c");
  }

  @Test
  public void testBufferedReadConfigurationKey() throws IOException {
    Configuration conf = new Configuration();
    conf.set(Constants.DAOS_POOL_UUID, DaosFSFactory.getPoolUuid());
    conf.set(Constants.DAOS_CONTAINER_UUID, DaosFSFactory.getContUuid());
    conf.set(Constants.DAOS_POOL_SVC, "0");

    boolean defaultEnabled = Constants.DEFAULT_BUFFERED_READ_ENABLED;
    conf.setBoolean(Constants.BUFFERED_READ_ENABLED, defaultEnabled ^ true);
    DaosFileSystem fs = new DaosFileSystem();
    fs.initialize(URI.create("daos:///"), conf);
    assert (fs.getBufferedReadEnabled() == defaultEnabled ^ true);
    fs.close();
    // if not set, should be default
    conf.unset(Constants.BUFFERED_READ_ENABLED);
    fs.initialize(URI.create("daos:///"), conf);
    assert (fs.getBufferedReadEnabled() == defaultEnabled);
    fs.close();
  }

  private void initializationTest(String initializationUri, String expectedUri) throws Exception{
    fs.initialize(URI.create(initializationUri), DaosUtils.getConfiguration());
    Assert.assertEquals(URI.create(expectedUri), fs.getUri());
  }


  @AfterClass
  public static void teardown() throws IOException {
    System.out.println("@AfterClass");
    fs.close();
  }
}
