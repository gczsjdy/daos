package com.intel.daos.client;

import org.junit.Assert;
import org.junit.Test;

import java.io.File;
import java.io.IOException;

public class DaosUtilsTest {

  @Test
  public void testReplaceForwardSlash(){
    String path = "\\abc\\de\\f\\";
    Assert.assertEquals("/abc/de/f", DaosUtils.normalize(path));

    path = "\\abc\\de\\f\\ghi\\\\jkl\\\\";
    Assert.assertEquals("/abc/de/f/ghi/jkl", DaosUtils.normalize(path));
  }

  @Test
  public void testEmptyPath(){
    String path = null;
    Assert.assertEquals("", DaosUtils.normalize(path));
    path = "";
    Assert.assertEquals("", DaosUtils.normalize(path));
    path = " ";
    Assert.assertEquals("", DaosUtils.normalize(path));
  }

  @Test
  public void testValidCharacter(){
    String path = "/";
    Assert.assertEquals("/", DaosUtils.normalize(path));
    path = "a0Ab1B_-";
    Assert.assertEquals("a0Ab1B_-", DaosUtils.normalize(path));
    path = "/a0Ab1B_-/1234567890/XYZ/_-/";
    Assert.assertEquals("/a0Ab1B_-/1234567890/XYZ/_-", DaosUtils.normalize(path));
  }

  @Test(expected = IllegalArgumentException.class)
  public void testValidIllegalCharacterWhiteSpace(){
    String path = "/abc /def";
    DaosUtils.normalize(path);
  }

  @Test(expected = IllegalArgumentException.class)
  public void testValidIllegalCharacterQuestionMark(){
    String path = "abc?";
    DaosUtils.normalize(path);
  }

  @Test
  public void testSplitPath(){
    String path = "/";
    String[] pc = DaosUtils.parsePath(DaosUtils.normalize(path));
    Assert.assertEquals(1, pc.length);
    Assert.assertEquals("/", pc[0]);

    path = "abc";
    pc = DaosUtils.parsePath(DaosUtils.normalize(path));
    Assert.assertEquals(1, pc.length);
    Assert.assertEquals("abc", pc[0]);

    path = "abc/";
    pc = DaosUtils.parsePath(DaosUtils.normalize(path));
    Assert.assertEquals(1, pc.length);
    Assert.assertEquals("abc", pc[0]);

    path = "abc/XYZ";
    pc = DaosUtils.parsePath(DaosUtils.normalize(path));
    Assert.assertEquals(2, pc.length);
    Assert.assertEquals("abc", pc[0]);
    Assert.assertEquals("XYZ", pc[1]);

    path = "/abc/XYZ/5TU-/ABC_";
    pc = DaosUtils.parsePath(DaosUtils.normalize(path));
    Assert.assertEquals(2, pc.length);
    Assert.assertEquals("/abc/XYZ/5TU-", pc[0]);
    Assert.assertEquals("ABC_", pc[1]);
  }

  @Test
  public void testUuidLength(){
    String id = DaosUtils.randomUUID();
    Assert.assertEquals(16, id.length());
  }
}






