package com.intel.daos.hadoop.fs;

import com.intel.daos.client.DaosFsClient;
import org.apache.commons.io.FileUtils;
import org.apache.hadoop.conf.Configuration;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

import java.io.File;
import java.io.InputStream;
import java.net.URI;

import static org.mockito.Mockito.*;

@RunWith(PowerMockRunner.class)
@PrepareForTest({DaosFsClient.DaosFsClientBuilder.class, DaosFileSystem.class})
@SuppressStaticInitializationFor("com.intel.daos.client.DaosFsClient")
public class DaosFileSystemTest {

  @Test
  public void testNewDaosFileSystemSuccessfulAndCreateRootDir() throws Exception {
    PowerMockito.mockStatic(DaosFsClient.class);
    DaosFsClient.DaosFsClientBuilder builder = mock(DaosFsClient.DaosFsClientBuilder.class);
    DaosFsClient client = mock(DaosFsClient.class);

    PowerMockito.whenNew(DaosFsClient.DaosFsClientBuilder.class).withNoArguments().thenReturn(builder);
    when(builder.poolId(anyString())).thenReturn(builder);
    when(builder.containerId(anyString())).thenReturn(builder);
    when(builder.ranks(anyString())).thenReturn(builder);
    when(builder.build()).thenReturn(client);

    DaosFileSystem fs = new DaosFileSystem();
    Configuration cfg = new Configuration();
    cfg.set(Constants.DAOS_POOL_UUID, "123");
    cfg.set(Constants.DAOS_CONTAINER_UUID, "123");
    cfg.set(Constants.DAOS_POOL_SVC, "0");
    fs.initialize(URI.create("daos://1234:56/"), cfg);
    Assert.assertEquals("daos://1234:56/user/"+System.getProperty("user.name"), fs.getWorkingDirectory().toString());
    verify(client, times(1)).mkdir("/user/"+System.getProperty("user.name"), true);
    fs.close();
  }

  @Test(expected = IllegalArgumentException.class)
  public void testNewDaosFileSystemFailedNoPoolId() throws Exception {
    PowerMockito.mockStatic(DaosFsClient.class);
    DaosFsClient.DaosFsClientBuilder builder = mock(DaosFsClient.DaosFsClientBuilder.class);
    DaosFsClient client = mock(DaosFsClient.class);

    PowerMockito.whenNew(DaosFsClient.DaosFsClientBuilder.class).withNoArguments().thenReturn(builder);
    when(builder.poolId(anyString())).thenReturn(builder);
    when(builder.containerId(anyString())).thenReturn(builder);
    when(builder.ranks(anyString())).thenReturn(builder);
    when(builder.build()).thenReturn(client);

    DaosFileSystem fs = new DaosFileSystem();
    Configuration cfg = new Configuration();
    fs.initialize(URI.create("daos://1234:56/root"), cfg);
    fs.close();
  }

  @Test(expected = IllegalArgumentException.class)
  public void testNewDaosFileSystemFailedNoContId() throws Exception {
    PowerMockito.mockStatic(DaosFsClient.class);
    DaosFsClient.DaosFsClientBuilder builder = mock(DaosFsClient.DaosFsClientBuilder.class);
    DaosFsClient client = mock(DaosFsClient.class);

    PowerMockito.whenNew(DaosFsClient.DaosFsClientBuilder.class).withNoArguments().thenReturn(builder);
    when(builder.poolId(anyString())).thenReturn(builder);
    when(builder.containerId(anyString())).thenReturn(builder);
    when(builder.ranks(anyString())).thenReturn(builder);
    when(builder.build()).thenReturn(client);

    DaosFileSystem fs = new DaosFileSystem();
    Configuration cfg = new Configuration();
    cfg.set(Constants.DAOS_POOL_UUID, "123");
    fs.initialize(URI.create("daos://1234:56/root"), cfg);
    fs.close();
  }

  @Test(expected = IllegalArgumentException.class)
  public void testNewDaosFileSystemFailedNoSvc() throws Exception {
    PowerMockito.mockStatic(DaosFsClient.class);
    DaosFsClient.DaosFsClientBuilder builder = mock(DaosFsClient.DaosFsClientBuilder.class);
    DaosFsClient client = mock(DaosFsClient.class);

    PowerMockito.whenNew(DaosFsClient.DaosFsClientBuilder.class).withNoArguments().thenReturn(builder);
    when(builder.poolId(anyString())).thenReturn(builder);
    when(builder.containerId(anyString())).thenReturn(builder);
    when(builder.ranks(anyString())).thenReturn(builder);
    when(builder.build()).thenReturn(client);

    DaosFileSystem fs = new DaosFileSystem();
    Configuration cfg = new Configuration();
    cfg.set(Constants.DAOS_POOL_UUID, "123");
    cfg.set(Constants.DAOS_CONTAINER_UUID, "123");
    fs.initialize(URI.create("daos://1234:56/root"), cfg);
    fs.close();
  }

  @Test
  public void testBufferedReadConfigurationKey() throws Exception {
    PowerMockito.mockStatic(DaosFsClient.class);
    DaosFsClient.DaosFsClientBuilder builder = mock(DaosFsClient.DaosFsClientBuilder.class);
    DaosFsClient client = mock(DaosFsClient.class);
    Configuration conf = new Configuration();

    PowerMockito.whenNew(DaosFsClient.DaosFsClientBuilder.class).withNoArguments().thenReturn(builder);
    when(builder.poolId(anyString())).thenReturn(builder);
    when(builder.containerId(anyString())).thenReturn(builder);
    when(builder.ranks(anyString())).thenReturn(builder);
    when(builder.build()).thenReturn(client);
    conf.set(Constants.DAOS_POOL_UUID, "123");
    conf.set(Constants.DAOS_CONTAINER_UUID, "123");
    conf.set(Constants.DAOS_POOL_SVC, "0");

    boolean defaultEnabled = Constants.DEFAULT_BUFFERED_READ_ENABLED;
    conf.setBoolean(Constants.DAOS_BUFFERED_READ_ENABLED, defaultEnabled ^ true);
    DaosFileSystem fs = new DaosFileSystem();
    fs.initialize(URI.create("daos://1234:56"), conf);
    Assert.assertTrue (fs.isBufferedReadEnabled() == defaultEnabled ^ true);
    fs.close();
    // if not set, should be default
    conf.unset(Constants.DAOS_BUFFERED_READ_ENABLED);
    fs.initialize(URI.create("daos://1234:56"), conf);
    Assert.assertTrue (fs.isBufferedReadEnabled() == defaultEnabled);
    fs.close();
  }

  @Test
  public void testLoadingConfig() throws Exception {
    Configuration cfg = new Configuration(false);
    cfg.addResource("daos-site.xml");
    String s = cfg.get("fs.defaultFS");
    Assert.assertEquals("daos://default:0", s);
    Assert.assertEquals(8388608, cfg.getInt("fs.daos.read.buffer.size", 0));
  }

  @Test
  public void testLoadingConfigFromStream() throws Exception {
    Configuration cfg = new Configuration(false);

    File tempFile = File.createTempFile("daos", "");
    try (InputStream is = this.getClass().getResourceAsStream("/daos-site.xml")) {
      FileUtils.copyInputStreamToFile(is, tempFile);
    }
    cfg.addResource(tempFile.toURI().toURL(), false);
    String s = cfg.get("fs.defaultFS");
    Assert.assertEquals("daos://default:0", s);
    Assert.assertEquals(8388608, cfg.getInt("fs.daos.read.buffer.size", 0));
  }

  @Test
  public void testLoadingConfigFromCoreSite() throws Exception {
    Configuration cfg = new Configuration(true);
    String s = cfg.get("fs.defaultFS");
    Assert.assertEquals("daos://id:1", s);
    Assert.assertEquals(8388608, cfg.getInt("fs.daos.read.buffer.size", 0));
  }
}
