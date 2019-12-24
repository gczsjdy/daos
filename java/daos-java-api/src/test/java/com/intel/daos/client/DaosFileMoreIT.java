package com.intel.daos.client;

import com.sun.security.auth.module.UnixSystem;
import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.IOException;

public class DaosFileMoreIT {

  private static String poolId;
  private static String contId;

  private static DaosFsClient client;

  @BeforeClass
  public static void setup()throws Exception{
    poolId = System.getProperty("pool_id", DaosFsClientTestBase.DEFAULT_POOL_ID);
    contId = System.getProperty("cont_id", DaosFsClientTestBase.DEFAULT_CONT_ID);
    client = DaosFsClientTestBase.prepareFs(poolId, contId);
  }

  @Test
  public void testNotExists()throws Exception{
    DaosFile file = client.getFile("/zjf1");
    Assert.assertFalse(file.exists());
  }

  @Test
  public void testNotExistsAfterDeletion()throws Exception{
    DaosFile file = client.getFile("/zjf2");
    file.createNewFile();
    Assert.assertTrue(file.exists());
    file.delete();
    Assert.assertFalse(file.exists());
  }

  @Test
  public void testIsDirectoryFalse() throws Exception{
    DaosFile file = client.getFile("/zjf3");
    file.createNewFile();
    Assert.assertFalse(file.isDirectory());
  }

  @Test
  public void tesMkdirAndVerify() throws Exception{
    DaosFile file = client.getFile("/dir11");
    file.mkdir();
    Assert.assertTrue(file.isDirectory());
  }

  @Test(expected = IOException.class)
  public void testGetChunkSizeOfDir() throws Exception {
    DaosFile file = client.getFile("/dir1");
    file.mkdir();
    file.getChunkSize();
  }

  @Test
  public void testGetStatAttributes()throws Exception{
    DaosFile file = client.getFile("/zjf4");
    file.createNewFile();
    StatAttributes attributes = file.getStatAttributes();

    UnixSystem system = new UnixSystem();
    int uid = (int)system.getUid();
    int gid = (int)system.getGid();

    Assert.assertTrue(attributes.getUid() == uid);
    Assert.assertTrue(attributes.getGid() == gid);
    Assert.assertTrue(attributes.isFile());
    Assert.assertTrue(attributes.getLength() == 0);
    Assert.assertTrue(attributes.getObjId() != 0);
    Assert.assertTrue(attributes.getBlockCnt() == 0);
    Assert.assertTrue(attributes.getBlockSize() == Constants.FILE_DEFAULT_CHUNK_SIZE);
    Assert.assertTrue(attributes.getMode() != 0);
    Assert.assertTrue(attributes.getAccessTime() != null);
    Assert.assertTrue(attributes.getModifyTime() != null);
    Assert.assertTrue(attributes.getCreateTime() != null);

    file = client.getFile("/zjf44");
    long time = System.currentTimeMillis();
    file.createNewFile(Constants.FILE_DEFAULT_FILE_MODE, DaosObjectType.OC_SX, 4096);
    attributes = file.getStatAttributes();
    Assert.assertTrue(attributes.getBlockSize() == 4096);

    long ctTime = DaosUtils.toMilliSeconds(attributes.getCreateTime());
    Assert.assertTrue(Math.abs(time - ctTime) < 1000);
    long mdTime = DaosUtils.toMilliSeconds(attributes.getModifyTime());
    Assert.assertTrue(Math.abs(time - mdTime) < 1000);
    long acTime = DaosUtils.toMilliSeconds(attributes.getAccessTime());
    Assert.assertTrue(Math.abs(time - acTime) < 1000);
  }

  @Test
  public void testSetGetExtAttribute() throws Exception{
    DaosFile file = client.getFile("/zjf5");
    file.createNewFile();
    file.setExtAttribute("att1", "xyz", Constants.SET_XATTRIBUTE_NO_CHECK);

    String value = file.getExtAttribute("att1", 3);
    Assert.assertEquals("xyz", value);

    value = file.getExtAttribute("att1", 4);
    Assert.assertEquals("xyz", value);
  }

  @Test(expected = DaosIOException.class)
  public void testRemoveExtAttribute()throws Exception{
    DaosFile file = client.getFile("/zjf6");
    file.mkdir();
    file.setExtAttribute("att1", "xyz", Constants.SET_XATTRIBUTE_CREATE);
    file.remoteExtAttribute("att1");
    file.getExtAttribute("att1", 3);
  }

  @Test
  public void testGetChunkSize()throws Exception{
    DaosFile file = client.getFile("/zjf7");
    file.createNewFile(0754, DaosObjectType.OC_SX, 2048);
    Assert.assertEquals(2048, file.getChunkSize());
  }

  @AfterClass
  public static void teardown()throws Exception{
    if(client != null){
      client.disconnect();
    }
  }
}
