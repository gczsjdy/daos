package com.intel.daos.hadoop.fs.multiple;

import com.intel.daos.hadoop.fs.CreateDaosFS;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class TestMultipleDaosOpenFile {
    static FileSystem daos ;
    static CreateDaosFS daosFS ;

    @BeforeClass
    public static void testDaosInit() throws IOException {
        daosFS=new CreateDaosFS();
        daosFS.getPool();
        daosFS.getContainer();
        daos=daosFS.getFs();
        Path f = new Path("/attempt_20191113114701_0024_m_000159_4855/part-00159-5404e5b0-a32d-41f9-bbb7-b9867e4e99ff-c000.csv");
        FSDataOutputStream status = daos.create(f);
    }

    @AfterClass
    public static void close(){
        System.out.println("@AfterClass");
        daosFS.close();
    }

    @Test
    public void testMultipleOpenFile() throws InterruptedException {
        ExecutorService executor = Executors.newFixedThreadPool(5);
        executor.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    daos=daosFS.getFs();
                    Path f = new Path("/attempt_20191113114701_0024_m_000159_4855/part-00159-5404e5b0-a32d-41f9-bbb7-b9867e4e99ff-c000.csv");
                    FSDataInputStream status = daos.open(f);
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        daos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        executor.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    daos=daosFS.getFs();
                    Path f = new Path("/attempt_20191113114701_0024_m_000159_4855/part-00159-5404e5b0-a32d-41f9-bbb7-b9867e4e99ff-c000.csv");
                    FSDataInputStream status = daos.open(f);
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        daos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        executor.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    daos=daosFS.getFs();
                    Path f = new Path("/attempt_20191113114701_0024_m_000159_4855/part-00159-5404e5b0-a32d-41f9-bbb7-b9867e4e99ff-c000.csv");
                    FSDataInputStream status = daos.open(f);
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        daos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        executor.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    daos=daosFS.getFs();
                    Path f = new Path("/attempt_20191113114701_0024_m_000159_4855/part-00159-5404e5b0-a32d-41f9-bbb7-b9867e4e99ff-c000.csv");
                    FSDataInputStream status = daos.open(f);
                } catch (IOException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        daos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        executor.submit(new Runnable() {
            @Override
            public void run() {
                try {
                    daos=daosFS.getFs();
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    try {
                        daos.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

            }
        });
        executor.shutdown();
        executor.awaitTermination(30, TimeUnit.SECONDS);
    }
}
