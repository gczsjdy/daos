package com.intel.daos.hadoop.fs;

import org.junit.Test;

import java.net.URI;
import static java.lang.System.out;

public class TestUri {

  @Test
  public void testUri()throws Exception{
    URI uri = URI.create("daos:///spark");
    out.println(uri.getScheme());
    out.println(uri.getAuthority());
    out.println(uri.getPath());
  }
}
