package com.intel.daos.hadoop.fs;

import com.intel.daos.client.DaosFsClient;
import org.apache.hadoop.conf.Configuration;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

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
}
