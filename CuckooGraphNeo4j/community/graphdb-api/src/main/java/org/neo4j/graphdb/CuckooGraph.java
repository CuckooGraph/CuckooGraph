package org.neo4j.graphdb;

import org.neo4j.annotations.api.PublicApi;

import java.io.*;
import java.util.ArrayList;
import java.util.Random;
import java.util.Arrays;
import java.util.List;
import java.util.Queue;
import java.util.LinkedList;
import java.util.ArrayDeque;
import java.util.Iterator;


@PublicApi
public class CuckooGraph {
    public String filePath;
    public int length;  //useless
    public int relationNum;
    public int recordNum;
    public int[] seeds;
    public static APHash[] hashes;
    public static Random random = new Random();
    public static Random rand = new Random();
    public int state;      
    public int ll1, ll2;
    //public int s1, s2;
    public int s1=0;
    public int s2=1;
    //public 
    public static int mykick;
    public static CGRelation mykick_w;
    public bCuckoo[] link = new bCuckoo[3];
    public static List<bSlot> blacklist = new ArrayList<>();
    public ArrayList<CGRelation> relations;
    public ArrayList<CGRecord> records;
    public static final int bup = 8;
    public static List<wEdge> sblacklist = new ArrayList<>();
    public static bSlot holekick;
    public static int bslotnum = 0;
    public static int sure = 9;
    public static int layernum = 0;
    public static int idth = 0;
    public static int blist=33;
    public static double rate=0.9;
    public static int sbl=128;
    public static int bbl=128;
    public static int us1=0;
    public static int us2=1;
    public static int u1=4;
    public static int u2=2;
    public static int link_num=3;
    public static int blink_num=3;
    public static int minc1=8;
    public static int minc2=4;
    public static double cthres=0.35;
    public static int bminc1=8;
    public static int bminc2=4;
    public static double bcthres=0.35;


    public CuckooGraph(int length, String filePath) {
        System.out.println("Processing Initialize ex");
        System.out.println("building " + " in " + filePath);
        this.filePath = filePath;
        this.length = length;
        state = -1;
        link[0] = null;
        link[1] = null;
        link[2] = null;
        ll1 = 4;
        ll2 = 2;
        s1 = 0;
        s2 = 1;
        sblacklist.clear();
        random.setSeed(10);
        seeds = new int[2];
        hashes = new APHash[2];
        for(int i = 0; i < 2; ++i){
            seeds[i] = random.nextInt();
            hashes[i] = new APHash();
            hashes[i].setSeed(seeds[i]);
        }
        relations = new ArrayList<>();
        records = new ArrayList<>();
        relationNum = 0;
        recordNum = 0;
        System.out.println("Finish Initialize ex");
    }

     
    public int mmhash(int key, int seed) {
        
        String src = String.valueOf(key);
        return hashes[seed].hash(src);
    }

    public class Edge {
        public int src;
        public int dst;
    }

    public class wEdge {
        public int src;
        public int dst;
        public CGRelation weight;
    
        public wEdge() {
            this.weight = null;
        }
    }

    public class Bucket {
        public int[] bslot;
        public CGRelation[] weight;

        Bucket(int bu) {
            bslot = new int[bu];
            weight = new CGRelation[bu];
            Arrays.fill(bslot, 0);
            //Arrays.fill(weight, 0);
            for(int r=0;r<bup;++r)
                weight[r]=null;
        }

        public int insert(int key, CGRelation weigh) {
            for (int i = 0; i < bup; ++i) {
                if (weight[i] == null) {
                   // int newRec = recordNum-1; 
                    bslot[i] = key;
                   // weight[i] = new CGRelation(0,bslot[i]);
                    weight[i] = weigh;
                    // relations.add(weight[i]);
                //    weight[i].nextRec = newRec;
                    return 1;
                }
            }
            return 0;
        }

        public CGRelation query(int key) {
            for (int i = 0; i < bup; ++i)
                if (weight[i]!=null&&bslot[i] == key)
                    return weight[i];
            return null;
        }

       
       // public int query_insert(int key) {
       //     return query_insert(key, 1);
       // }

        public int query_insert(int key, CGRelation weigh) {
            for (int i = 0; i < bup; ++i)
                if (weight[i]!=null&&bslot[i] == key) {
                    // int newRec = recordNum-1;
                    // weigh.nextRec = weight[i].nextRec;
                    // weight[i].nextRec = newRec;
                    
                    return 1;
                }
            return 0;
        }

        public boolean tq(int key) {
            for (int i = 0; i < bup; ++i)
                if (weight[i]!=null&&bslot[i] == key)
                    return true;
            return false;
        }
    }

    public class Layer {
        public Bucket[] layer;
        public int length;
        public int seed;

        public Layer() {}

        public Layer(int num, int s) {
            this.layer = new Bucket[num];
            this.length = num;
            this.seed = s;
            for (int i = 0; i < num; i++) {
                this.layer[i] = new Bucket(bup); 
            }
        }

        //public int del(int key) {
        //    return layer[mmhash(key, seed) % length].del(key);
        //}

        public CGRelation query(int key) {
            return layer[mmhash(key, seed) % length].query(key);
        }

        public int insert(int key, CGRelation weight) {
            return layer[mmhash(key, seed) % length].insert(key, weight);
        }

        //public int query_insert(int key) {
        //    return query_insert(key, 1);
        //}

        public int query_insert(int key, CGRelation weight) {
            return layer[mmhash(key, seed) % length].query_insert(key, weight);
        }
    }

    public class Cuckoo {
        public int len1;
        public int len2;
        public int seed1, seed2;
        public int num;
        public Layer layer1;
        public Layer layer2;
        public static final int kicktimes = 500; 
        // public static final Random rand = new Random();
    
        public Cuckoo(int l1, int l2, int s1, int s2) {
            this.len1 = l1;
            this.len2 = l2;
            this.seed1 = s1;
            this.seed2 = s2;
            this.num = 0;
            this.layer1 = new Layer(l1, s1);
            this.layer2 = new Layer(l2, s2);
        }
    
        //public boolean insert(int item) {
        //    return insert(item, 1);
        //}
    
        public boolean insert(int item, CGRelation weight) {
            int ir = layer1.insert(item, weight);
            if (ir != 0) {
                if (ir == 1)
                    ++num;
                return true;
            }
            ir = layer2.insert(item, weight);
            if (ir != 0) {
                if (ir == 1)
                    ++num;
                return true;
            }
            int times = kicktimes;
            boolean flag = true;
            int pos1 = mmhash(item, seed1) % len1;
            int pos2 = mmhash(item, seed2) % len2;
            int kick = item;
            CGRelation kick_w = weight;
            //CGRelation kick_w = null;
            int ins = item;
            CGRelation ins_w = weight;
            //CGRelation ins_w = null;
            while (times > 0) {
                if (flag) {
                    int xx = rand.nextInt();
                    kick = layer1.layer[pos1].bslot[Math.floorMod(xx, bup)];
                    kick_w = layer1.layer[pos1].weight[Math.floorMod(xx, bup)];
                    layer1.layer[pos1].bslot[Math.floorMod(xx, bup)] = ins;
                    layer1.layer[pos1].weight[Math.floorMod(xx, bup)] = ins_w;
                    int f = layer2.insert(kick, kick_w);
                    if (f != 0) {
                        if (f == 1)
                            ++num;
                        return true;
                    }
                    --times;
                    flag = false;
                    ins = kick;
                    ins_w = kick_w;
                    pos2 = mmhash(ins, seed2) % len2;
                } else {
                    int xx = rand.nextInt();
                    kick = layer2.layer[pos2].bslot[Math.floorMod(xx, bup)];
                    kick_w = layer2.layer[pos2].weight[Math.floorMod(xx, bup)];
                    layer2.layer[pos2].bslot[Math.floorMod(xx, bup)] = ins;
                    layer2.layer[pos2].weight[Math.floorMod(xx, bup)] = ins_w;
                    int f = layer1.insert(kick, kick_w);
                    if (f != 0) {
                        if (f == 1)
                            ++num;
                        return true;
                    }
                    --times;
                    flag = true;
                    ins = kick;
                    ins_w = kick_w;
                    pos1 = mmhash(ins, seed1) % len1;
                }
            }
    
            mykick = ins;
            mykick_w = ins_w;
            return false;
        }
    
        public CGRelation query(int item) {
            CGRelation q = layer1.query(item);
            if (q != null) return q;
            q = layer2.query(item);
            if (q != null) return q;
            return q;
        }
    
        //public int query_insert(int item) {
        //    return query_insert(item, 1);
        //}
    
        public int query_insert(int item, CGRelation weight) {
            int q = layer1.query_insert(item, weight);
            if (q != 0) return q;
            q = layer2.query_insert(item, weight);
            if (q != 0) return q;
            return q;
        }
    
        
    }
    

    public class bSlot {
        public Cuckoo[] slot = new Cuckoo[3];
        public int src;
        public int pt;
    
        public bSlot() {
            this.src = 0;
            this.pt = 0;
            for (int i = 0; i < slot.length; i++) {
                slot[i] = null;
            }
        }
    
        public Cuckoo merge(Cuckoo c0, Cuckoo c1, Cuckoo c2) {
        //    if(src==-274725900)System.out.println("MERGE");
            int l1 = c0.len1;
            int l2 = c0.len2;
            Queue<Integer> mq = new LinkedList<>();
            Queue<CGRelation> mq_w = new LinkedList<>();
            Cuckoo c = new Cuckoo(l1 * 2, l2 * 2, us1, us2);
            
            for (int i = 0; i < l1; i++) {
                for(int j=0;j<bup;++j){
                c.layer1.layer[i].bslot[j] = c0.layer1.layer[i].bslot[j];
                c.layer1.layer[i].weight[j] = c0.layer1.layer[i].weight[j];
                }
            }

            
            for (int i = 0; i < l1; i++) {
                for(int j=0;j<bup;++j){
                    c.layer1.layer[i+l1].bslot[j] = c0.layer1.layer[i].bslot[j];
                    c.layer1.layer[i+l1].weight[j] = c0.layer1.layer[i].weight[j];
                    }
            }

            
            for (int i = 0; i < l2; i++) {
                for(int j=0;j<bup;++j){
                 c.layer2.layer[i].bslot[j] = c0.layer2.layer[i].bslot[j];
                 c.layer2.layer[i].weight[j] = c0.layer2.layer[i].weight[j];
                }
            }

            
            for (int i = 0; i < l2; i++) {
                for(int j=0;j<bup;++j){
                    c.layer2.layer[i+l2].bslot[j] = c0.layer2.layer[i].bslot[j];
                    c.layer2.layer[i+l2].weight[j] = c0.layer2.layer[i].weight[j];
                   }
            }

            c.num = c0.num;
            for (int i = 0; i < l1; ++i) {
                int k = i + l1;
                for (int j = 0; j < bup; ++j) {
                    if (c.layer1.layer[i].weight[j] == null) continue;
          //          if(src==-274725900)System.out.println("IN1");
                    if (i != mmhash(c.layer1.layer[i].bslot[j], us1) % (l1 * 2)) {
                        // if(src==-274725900){
                        //    if(k!=mmhash(c.layer1.layer[i].bslot[j], us1) % (l1 * 2)) {
                        //     System.out.println("???");
                        //     System.out.println("c.layer1.layer[i].bslot[j]: "+c.layer1.layer[i].bslot[j]);
                        //     System.out.println(i == mmhash(c.layer1.layer[i].bslot[j], us1) % (l1 ));
                        // }
                        // }
                        c.layer1.layer[i].bslot[j] = 0;
                        c.layer1.layer[i].weight[j] = null;
                    } else {
                        c.layer1.layer[k].bslot[j] = 0;
                        c.layer1.layer[k].weight[j] = null;
                    }
                }
            }
            for (int i = 0; i < l1 / 2; ++i) {
                for (int j = 0; j < bup; ++j) {
                    if (c1.layer1.layer[i].weight[j] == null) continue;
                    if (c.layer1.layer[mmhash(c1.layer1.layer[i].bslot[j], us1) % (l1 * 2)].insert(c1.layer1.layer[i].bslot[j], c1.layer1.layer[i].weight[j]) == 0) {
                        mq.add(c1.layer1.layer[i].bslot[j]);
                        mq_w.add(c1.layer1.layer[i].weight[j]);
                    } else {
                        c.num++;
                    }
                }
                for (int j = 0; j < bup; ++j) {
                    if (c2.layer1.layer[i].weight[j] == null) continue;
                    if (c.layer1.layer[mmhash(c2.layer1.layer[i].bslot[j], us1) % (l1 * 2)].insert(c2.layer1.layer[i].bslot[j], c2.layer1.layer[i].weight[j]) == 0) {
                        mq.add(c2.layer1.layer[i].bslot[j]);
                        mq_w.add(c2.layer1.layer[i].weight[j]);
                    } else {
                        c.num++;
                    }
                }
            }
            for (int i = 0; i < l2; ++i) {
                int k = i + l2;
                for (int j = 0; j < bup; ++j) {
                    if (c.layer2.layer[i].weight[j] == null) continue;
                //    if(src==-274725900)System.out.println("IN2");
                    if (i != mmhash(c.layer2.layer[i].bslot[j], us2) % (l2 * 2)) {
                        // if(src==-274725900){
                        //     if(k!=mmhash(c.layer2.layer[i].bslot[j], us2) % (l2 * 2)) System.out.println("???");
                        //  }
                        c.layer2.layer[i].bslot[j] = 0;
                        c.layer2.layer[i].weight[j] = null;
                    } else {
                        c.layer2.layer[k].bslot[j] = 0;
                        c.layer2.layer[k].weight[j] = null;
                    }
                }
            }
            for (int i = 0; i < l2 / 2; ++i) {
                for (int j = 0; j < bup; ++j) {
                    if (c1.layer2.layer[i].weight[j] == null) continue;
                    if (c.layer2.layer[mmhash(c1.layer2.layer[i].bslot[j], us2) % (l2 * 2)].insert(c1.layer2.layer[i].bslot[j], c1.layer2.layer[i].weight[j]) == 0) {
                        mq.add(c1.layer2.layer[i].bslot[j]);
                        mq_w.add(c1.layer2.layer[i].weight[j]);
                    } else {
                        c.num++;
                    }
                }
                for (int j = 0; j < bup; ++j) {
                    if (c2.layer2.layer[i].weight[j] == null) continue;
                    if (c.layer2.layer[mmhash(c2.layer2.layer[i].bslot[j], us2) % (l2 * 2)].insert(c2.layer2.layer[i].bslot[j], c2.layer2.layer[i].weight[j]) == 0) {
                        mq.add(c2.layer2.layer[i].bslot[j]);
                        mq_w.add(c2.layer2.layer[i].weight[j]);
                    } else {
                        c.num++;
                    }
                }
            }
        //    if(src==-274725900){System.out.println("mq.size(): "+mq.size());System.out.println("l1 * 2: "+l1 * 2);}
        //    if(src==-274725900){System.out.println("sblacklist.size(): "+sblacklist.size());}
            while (!mq.isEmpty()) {
                if (!c.insert(mq.peek(), mq_w.peek())) {
                    wEdge now_insert = new wEdge();
                    now_insert.src = src;
                    now_insert.dst = mykick;
                    now_insert.weight = mykick_w;
                    sblacklist.add(now_insert);
                }
                mq.poll();
                mq_w.poll();
            }
          //  if(src==-274725900){System.out.println("sblacklist.size(): "+sblacklist.size());}
            return c;
        }
    
        // public int query_insert(Edge e, CGRelation weight) {
        //     if (e.src != src) {
        //         return 0;
        //     }
        //     if (pt < 0) {
        //         for (int i = 0; i < 3; ++i) {
        //             if (slot[i] != null && slot[i].query_insert(e.dst, weight) != 0) return i + 9;
        //         }
        //         for (wEdge it : sblacklist) {
        //             if (it.src == e.src && it.dst == e.dst) {
        //                 //it.weight++;
        //                 // int newRec = recordNum-1; 
        //                 // it.weight.nextRec = newRec;
        //                 return blist;
        //             }
        //         }
        //         return 0;
        //     } else {
        //         return 0;
        //     }
        // }
    
        public int insert(Edge EE, CGRelation weight) {
            if (pt == 0) {
                src = EE.src;
                slot[0] = new Cuckoo(u1, u2, us1, us2);
                if (!slot[0].insert(EE.dst, weight)) {
                    wEdge now_insert = new wEdge();
                    now_insert.src = EE.src;
                    now_insert.dst = mykick;
                    now_insert.weight = mykick_w;
                    sblacklist.add(now_insert);
                }
                pt = -1;
             //   if(src==-274725900&&src == EE.src)System.out.println("D1");
                return pt;
            } else if (src != EE.src) {
           //     if(src==-274725900&&src == EE.src)System.out.println("D2");
                return 0;
            } else if (pt < 0) {
                int state = -1;
                Edge e = new Edge();
                e.src = EE.src;
                e.dst = EE.dst;
                CGRelation e_w = weight;
                // CGRelation e_w = new CGRelation(EE.src,EE.dst);
                // e_w.nextRec= recordNum-1; 
                if (sblacklist.size() < sbl) {
                    for (int i = 2; i >= 0; --i) {
                        if (slot[i] != null) {
                            ++state;
                            if (slot[i].num < (slot[i].len1 + slot[i].len2) * bup * rate) {
                                if (!slot[i].insert(EE.dst, weight)) {
                                    wEdge now_insert = new wEdge();
                                    now_insert.src = EE.src;
                                    now_insert.dst = mykick;
                                    now_insert.weight = mykick_w;
                                    sblacklist.add(now_insert);
                                }
                             //   if(src==-274725900&&src == EE.src)System.out.println("D3");
                                return i + 9;
                            }
                        }
                    }
                } else {
                    boolean fff = false;
                    for (int i = 2; i >= 0; --i) {
                        if (slot[i] != null) {
                            ++state;
                            if (slot[i].num < (slot[i].len1 + slot[i].len2) * bup * rate && !fff) {
                                if (!slot[i].insert(EE.dst, weight)) {
                                    fff = true;
                                    e.src = EE.src;
                                    e.dst = mykick;
                                    e_w = mykick_w;
                                } else {
                                 //   if(src==-274725900&&src == EE.src)System.out.println("D4");
                                    return i + 9;
                                }
                            }
                        }
                    }
                }
                if (state == 0) {
                    slot[1] = new Cuckoo(slot[0].len1 / 2, slot[0].len2 / 2, us1, us2);
                    Queue<Integer> qq = new LinkedList<>();
                    Queue<CGRelation> qq_w = new LinkedList<>();
                    Iterator<wEdge> iterator = sblacklist.iterator(); 
                    while (iterator.hasNext()) {
                         wEdge it = iterator.next();
                         if (it.src == src) {
                           if (slot[1].insert(it.dst, it.weight)==false) {
                                qq.add(mykick);
                                 qq_w.add(mykick_w);
                            }
                        iterator.remove(); 
                        }
                    }
    
                    while (!qq.isEmpty()) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = src;
                        now_insert.dst = qq.poll();
                        now_insert.weight = qq_w.poll();
                        sblacklist.add(now_insert);
                    }
                    if (slot[1].insert(e.dst, e_w)==false) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = e.src;
                        now_insert.dst = mykick;
                        now_insert.weight = mykick_w;
                        sblacklist.add(now_insert);
                    }
                //    if(src==-274725900&&src == EE.src)System.out.println("D5");
                    return 10;
                }
                if (state == 1) {
                    slot[2] = new Cuckoo(slot[1].len1, slot[1].len2, us1, us2);
                    Queue<Integer> qq = new LinkedList<>();
                    Queue<CGRelation> qq_w = new LinkedList<>();
                    /*for (wEdge it : sblacklist) {
                        if (it.src == src) {
                            if (!slot[2].insert(it.dst, it.weight)) {
                                qq.add(mykick);
                                qq_w.add(mykick_w);
                            }
                            sblacklist.remove(it);
                        }
                    }*/
                    Iterator<wEdge> iterator = sblacklist.iterator(); // 
                    while (iterator.hasNext()) {
                        wEdge it = iterator.next();
                        if (it.src == src) {
                           if (slot[2].insert(it.dst, it.weight)==false) {
                               qq.add(mykick);
                               qq_w.add(mykick_w);
                            }
                        iterator.remove(); 
                        }
                    }
    
                    while (!qq.isEmpty()) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = src;
                        now_insert.dst = qq.poll();
                        now_insert.weight = qq_w.poll();
                        sblacklist.add(now_insert);
                    }
                    if (slot[2].insert(e.dst, e_w)==false) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = e.src;
                        now_insert.dst = mykick;
                        now_insert.weight = mykick_w;
                        sblacklist.add(now_insert);
                    }
             //       if(src==-274725900&&src == EE.src)System.out.println("D6");
                    return 11;
                }
                if (state == 2) {
                    Cuckoo cm = merge(slot[0], slot[1], slot[2]);
                    slot[1] = null;
                    slot[2] = null;
                    slot[0] = cm;
                    slot[1] = new Cuckoo(slot[0].len1 / 2, slot[0].len2 / 2, us1, us2);
                    Queue<Integer> qq = new LinkedList<>();
                    Queue<CGRelation> qq_w = new LinkedList<>();
                    Iterator<wEdge> iterator = sblacklist.iterator(); 
                    while (iterator.hasNext()) {
                         wEdge it = iterator.next(); 
                         if (it.src == src) {
                           if (slot[1].insert(it.dst, it.weight)==false) {
                              qq.add(mykick);  
                              qq_w.add(mykick_w);
                            }   
                            iterator.remove(); 
                        }
                    }
    
                    while (!qq.isEmpty()) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = src;
                        now_insert.dst = qq.poll();
                        now_insert.weight = qq_w.poll();
                        sblacklist.add(now_insert);
                    }
                    if (slot[1].insert(e.dst, e_w)==false) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = e.src;
                        now_insert.dst = mykick;
                        now_insert.weight = mykick_w;
                        sblacklist.add(now_insert);
                        // if(src==-274725900&&src == EE.src){
                        //     System.out.println("D7");
                        //     System.out.println("e.dst: "+e.dst);
                        //     System.out.println("mykick: "+mykick);
                        //     System.out.println("EE.dst: "+EE.dst);
                        //     System.out.println("slot[1].len1: "+slot[1].len1);
                        //     System.out.println("slot[1].num: "+slot[1].num);
                        // }
                    }
                    // if(src==-274725900&&src == EE.src){
                    // System.out.println("D7");
                    // System.out.println("e.dst: "+e.dst);
                    // System.out.println("mykick: "+mykick);
                    // System.out.println("EE.dst: "+EE.dst);
                    // System.out.println("slot[1].len1: "+slot[1].len1);
                    // System.out.println("slot[1].num: "+slot[1].num);
                    // }
                    return 10;
                }
                if (state == -1) {
                    slot[0] = new Cuckoo(u1, u2, us1, us2);
                    if (slot[0].insert(e.dst, e_w)==false) {
                        wEdge now_insert = new wEdge();
                        now_insert.src = e.src;
                        now_insert.dst = mykick;
                        now_insert.weight = mykick_w;
                        sblacklist.add(now_insert);
                    }
                //    if(src==-274725900&&src == EE.src)System.out.println("D8");
                    return 9;
                }
                throw new AssertionError();
            } 
            throw new AssertionError();
        }
    
        public CGRelation query(Edge e) {
            if (e.src != src) {
                return null;
            }
            if (pt < 0) {
                for (int i = 0; i < 3; ++i) {
                    if (slot[i] != null) {
                        CGRelation aa = slot[i].query(e.dst);
                        if (aa != null) return aa;
                    }
                }
                for (wEdge it : sblacklist) {
                    if (it.src == e.src && it.dst == e.dst) {
                        return it.weight;
                    }
                }
                return null;
            } else {
                //for (int i = 0; i < 3; ++i) {
                //    if (slot[i] != null && slot[i].query(e.dst) != 0) return slot[i].query(e.dst);
                //}
                return null;
            }
        }
    
        public boolean isSlotEmpty() {
            return pt == 0;
        }
    }
 

    public class bBucket { 
        public bSlot[] bslot = new bSlot[bup];
        public bBucket(){
            for(int i=0;i<bup;++i)
            bslot[i]=new bSlot();
        }
        
        public int insert(Edge e, CGRelation weight) {
            
            for (int i = 0; i < bup; ++i) {
                if (bslot[i].src == e.src&&!bslot[i].isSlotEmpty()) {
                    bslot[i].insert(e, weight);
                    return -1;
                }
            }
            
            
            for (int i = 0; i < bup; ++i) {
                if (bslot[i].insert(e, weight)!=0) {
                    return 1;
                }
            }
    
            return 0;
        }
    
       
        public boolean insert(bSlot e) {
            for (int i = 0; i < bup; ++i) {
                if (bslot[i].isSlotEmpty()) {
                    bslot[i] = e;
                    return true;
                }
            }
            return false;
        }
    
       
        public CGRelation query(Edge e) {
            for (int i = 0; i < bup; ++i) {
                if (bslot[i].src == e.src&&!bslot[i].isSlotEmpty()) {
                    return bslot[i].query(e);
                }
            }
            return null;
        }
    
        
        public int insert_query3(Edge e, CGRelation weight) {
            for (int i = 0; i < bup; ++i) {
                if (bslot[i].src == e.src&&bslot[i].pt!=0) {
                    
                    bslotnum = i;
                    return 1;
                }
            }
            return 0;
        }
    
        
        public int query3(Edge e) {
            for (int i = 0; i < bup; ++i) {
                if (bslot[i].src == e.src&&!bslot[i].isSlotEmpty()) {
                    if (bslot[i].query(e)!=null) {
                        return sure; 
                    }
                    bslotnum = i;
                    return 1;
                }
            }
            return 0;
        }
    }

    public class bLayer {
        public bBucket[] layer;
        public int length;
        public int seed;
    
        
        public bLayer() {}
    
        
        public bLayer(int num, int s) {
            layer = new bBucket[num];  
            length = num;
            seed = s;
            for (int i = 0; i < num; i++) {
                layer[i] = new bBucket();  
            }
        }
    
        
        public CGRelation query(Edge e) {
            int index = (int)(mmhash(e.src, seed) % length);
            return layer[index].query(e);
        }
    
        
        public int insert(Edge e, CGRelation weight) {
            int index = (int)(mmhash(e.src, seed) % length);
            return layer[index].insert(e, weight);
        }
    
        
        public boolean insert(bSlot e) {
            int index = (int)(mmhash(e.src, seed) % length);
            return layer[index].insert(e);
        }
    }


    public class bCuckoo {
        public int len1;
        public int len2;
        public int seed1, seed2;
        public int num;
        public bLayer layer1;
        public bLayer layer2;
       
        public bCuckoo(int l1, int l2, int s1, int s2) {
            len1 = l1;
            len2 = l2;
            seed1 = s1;
            seed2 = s2;
            num = 0;
            layer1 = new bLayer(l1, s1);
            layer2 = new bLayer(l2, s2);
        }
    
        
        public boolean insert(Edge item, CGRelation weight) {
            int ir = layer1.insert(item, weight);
            if (ir != 0) {
                if (ir == 1) {
                    ++num;
                }
                return true;
            }
            ir = layer2.insert(item, weight);
            if (ir != 0) {
                if (ir == 1) {
                    ++num;
                }
                return true;
            }
    
            int times = 500;
            boolean flag = true;
            int pos1 = (int)(mmhash(item.src, seed1) % len1);
            int pos2 = (int)(mmhash(item.src, seed2) % len2);
            bSlot kick;
            bSlot ins = new bSlot();
            ins.insert(item, weight);
          //  if(ins.query(item)==null)System.out.println("what!");
            Random rand = new Random();
    
            while (times > 0) {
                int xx = rand.nextInt();
                if (flag) {
                    kick = layer1.layer[pos1].bslot[Math.floorMod(xx, bup)];
                    layer1.layer[pos1].bslot[Math.floorMod(xx, bup)] = ins;
                    if (layer2.insert(kick)) {
                        ++num;
                        return true;
                    }
                    --times;
                    flag = false;
                    ins = kick;
                    pos2 = (int)(mmhash(ins.src, seed2) % len2);
                } else {
                    kick = layer2.layer[pos2].bslot[Math.floorMod(xx, bup)];
                    layer2.layer[pos2].bslot[Math.floorMod(xx, bup)] = ins;
                    if (layer1.insert(kick)) {
                        ++num;
                        return true;
                    }
                    --times;
                    flag = true;
                    ins = kick;
                    pos1 = (int)(mmhash(ins.src, seed1) % len1);
                }
            }
            holekick = ins;
            return false;
        }
    
        
        public boolean insert(bSlot item) {
            if (layer1.insert(item)) {
                ++num;
                return true;
            }
            if (layer2.insert(item)) {
                ++num;
                return true;
            }
    
            int times = 500;
            boolean flag = true;
            int pos1 = (int)(mmhash(item.src, seed1) % len1);
            int pos2 = (int)(mmhash(item.src, seed2) % len2);
            bSlot kick = item;
            bSlot ins = item;
            Random rand = new Random();
    
            while (times > 0) {
                int xx = rand.nextInt();
                if (flag) {
                    kick = layer1.layer[pos1].bslot[Math.floorMod(xx, bup)];
                    layer1.layer[pos1].bslot[Math.floorMod(xx, bup)] = ins;
                    if (layer2.insert(kick)) {
                        ++num;
                        return true;
                    }
                    --times;
                    flag = false;
                    ins = kick;
                    pos2 = (int)(mmhash(ins.src, seed2) % len2);
                } else {
                    kick = layer2.layer[pos2].bslot[Math.floorMod(xx, bup)];
                    layer2.layer[pos2].bslot[Math.floorMod(xx, bup)] = ins;
                    if (layer1.insert(kick)) {
                        ++num;
                        return true;
                    }
                    --times;
                    flag = true;
                    ins = kick;
                    pos1 = (int)(mmhash(ins.src, seed1) % len1);
                }
            }
            holekick = ins;
            return false;
        }
    
        
        public CGRelation query(Edge item) {
            CGRelation ans = layer1.query(item);
            if (ans != null) {
                return ans;
            }
            return layer2.query(item);
        }
    
        
        public int insert_query3(Edge item, CGRelation weight) {
            int ha = (int)(mmhash(item.src, seed1) % len1);
            int ans = layer1.layer[ha].insert_query3(item, weight);
            if (ans == sure) return sure;
            if (ans != 0) {
                layernum = 1;
                idth = ha;
                return 1;
            }
    
            ha = (int)(mmhash(item.src, seed2) % len2);
            ans = layer2.layer[ha].insert_query3(item, weight);
            if (ans == sure) return sure;
            if (ans != 0) {
                layernum = 2;
                idth = ha;
                return 2;
            }
            return 0;
        }
    
        
        public int query3(Edge item) {
            int ha = (int)(mmhash(item.src, seed1) % len1);
            int ans = layer1.layer[ha].query3(item);
            if (ans == sure) return sure;
            if (ans != 0) {
                layernum = 1;
                idth = ha;
                return 1;
            }
    
            ha = (int)(mmhash(item.src, seed2) % len2);
            ans = layer2.layer[ha].query3(item);
            if (ans == sure) return sure;
            if (ans != 0) {
                layernum = 2;
                idth = ha;
                return 2;
            }
            return 0;
        }
    }

    public bCuckoo merge(bCuckoo c0, bCuckoo c1, bCuckoo c2) {
//        System.out.println("bCuckoomerge!");
        int l1 = c0.len1;
        int l2 = c0.len2;
        Queue<bSlot> mq = new ArrayDeque<>();
        bCuckoo c = new bCuckoo(l1 * 2, l2 * 2, s1, s2);
    
        // // Layer 1 memcpy equivalent
        // System.arraycopy(c0.layer1.layer, 0, c.layer1.layer, 0, l1);
        // System.arraycopy(c0.layer1.layer, 0, c.layer1.layer, l1, l1);
    
        // // Layer 2 memcpy equivalent
        // System.arraycopy(c0.layer2.layer, 0, c.layer2.layer, 0, l2);
        // System.arraycopy(c0.layer2.layer, 0, c.layer2.layer, l2, l2);
        // Layer 1 memcpy equivalent
        for (int i = 0; i < l1; i++) {
            for(int j=0;j<bup;++j){
             c.layer1.layer[i].bslot[j].src = c0.layer1.layer[i].bslot[j].src;
             c.layer1.layer[i].bslot[j].pt = c0.layer1.layer[i].bslot[j].pt;
             c.layer1.layer[i].bslot[j].slot[0] = c0.layer1.layer[i].bslot[j].slot[0];
             c.layer1.layer[i].bslot[j].slot[1] = c0.layer1.layer[i].bslot[j].slot[1];
             c.layer1.layer[i].bslot[j].slot[2] = c0.layer1.layer[i].bslot[j].slot[2];
            }
        }
        for (int i = 0; i < l1; i++) {
            for(int j=0;j<bup;++j){
            // c.layer1.layer[i + l1].bslot[j] = c0.layer1.layer[i].bslot[j];
             c.layer1.layer[i+ l1].bslot[j].src = c0.layer1.layer[i].bslot[j].src;
             c.layer1.layer[i+ l1].bslot[j].pt = c0.layer1.layer[i].bslot[j].pt;
             c.layer1.layer[i+ l1].bslot[j].slot[0] = c0.layer1.layer[i].bslot[j].slot[0];
             c.layer1.layer[i+ l1].bslot[j].slot[1] = c0.layer1.layer[i].bslot[j].slot[1];
             c.layer1.layer[i+ l1].bslot[j].slot[2] = c0.layer1.layer[i].bslot[j].slot[2];
        }
        }

        // Layer 2 memcpy equivalent
        for (int i = 0; i < l2; i++) {
           //  c.layer2.layer[i] = c0.layer2.layer[i];
           for(int j=0;j<bup;++j){
            c.layer2.layer[i].bslot[j].src = c0.layer2.layer[i].bslot[j].src;
            c.layer2.layer[i].bslot[j].pt = c0.layer2.layer[i].bslot[j].pt;
            c.layer2.layer[i].bslot[j].slot[0] = c0.layer2.layer[i].bslot[j].slot[0];
            c.layer2.layer[i].bslot[j].slot[1] = c0.layer2.layer[i].bslot[j].slot[1];
            c.layer2.layer[i].bslot[j].slot[2] = c0.layer2.layer[i].bslot[j].slot[2];
           }
        }
        for (int i = 0; i < l2; i++) {
              //c.layer2.layer[i + l2] = c0.layer2.layer[i];
              for(int j=0;j<bup;++j){
                c.layer2.layer[i+l2].bslot[j].src = c0.layer2.layer[i].bslot[j].src;
                c.layer2.layer[i+l2].bslot[j].pt = c0.layer2.layer[i].bslot[j].pt;
                c.layer2.layer[i+l2].bslot[j].slot[0] = c0.layer2.layer[i].bslot[j].slot[0];
                c.layer2.layer[i+l2].bslot[j].slot[1] = c0.layer2.layer[i].bslot[j].slot[1];
                c.layer2.layer[i+l2].bslot[j].slot[2] = c0.layer2.layer[i].bslot[j].slot[2];
               }
        }
    
        c.num = c0.num;
    
        // Process layer 1
        for (int i = 0; i < l1; ++i) {
            int k = i + l1;
            for (int j = 0; j < bup; ++j) {
                if (c.layer1.layer[i].bslot[j].isSlotEmpty()) continue;
                if (i != mmhash(c.layer1.layer[i].bslot[j].src, s1) % (l1 * 2)) {
                    c.layer1.layer[i].bslot[j].src = 0;
                    c.layer1.layer[i].bslot[j].pt = 0;
                    c.layer1.layer[i].bslot[j].slot = new Cuckoo[3];
                    c.layer1.layer[i].bslot[j].slot[0]=null;
                    c.layer1.layer[i].bslot[j].slot[1]=null;
                    c.layer1.layer[i].bslot[j].slot[2]=null;
                } else {
                    c.layer1.layer[k].bslot[j].src = 0;
                    c.layer1.layer[k].bslot[j].pt = 0;
                    c.layer1.layer[k].bslot[j].slot = new Cuckoo[3];
                    c.layer1.layer[k].bslot[j].slot[0] =null;
                    c.layer1.layer[k].bslot[j].slot[1] =null;
                    c.layer1.layer[k].bslot[j].slot[2] =null;
                }
            }
        }
    
        // Merge c1 and c2 into c (layer 1)
        for (int i = 0; i < l1 / 2; ++i) {
            for (int j = 0; j < bup; ++j) {
                if (c1.layer1.layer[i].bslot[j].isSlotEmpty()) continue;
                if (!c.layer1.layer[mmhash(c1.layer1.layer[i].bslot[j].src, s1) % (l1 * 2)].insert(c1.layer1.layer[i].bslot[j])) {
                    mq.add(c1.layer1.layer[i].bslot[j]);
                } else {
                    c.num += 1;
                }
            }
    
            for (int j = 0; j < bup; ++j) {
                if (c2.layer1.layer[i].bslot[j].isSlotEmpty()) continue;
                if (!c.layer1.layer[mmhash(c2.layer1.layer[i].bslot[j].src, s1) % (l1 * 2)].insert(c2.layer1.layer[i].bslot[j])) {
                    mq.add(c2.layer1.layer[i].bslot[j]);
                } else {
                    c.num += 1;
                }
            }
        }
    
        // Process layer 2
        for (int i = 0; i < l2; ++i) {
            int k = i + l2;
            for (int j = 0; j < bup; ++j) {
                if (c.layer2.layer[i].bslot[j].isSlotEmpty()) continue;
                if (i != mmhash(c.layer2.layer[i].bslot[j].src, s2) % (l2 * 2)) {
                    c.layer2.layer[i].bslot[j].src = 0;
                    c.layer2.layer[i].bslot[j].pt = 0;
                    c.layer2.layer[i].bslot[j].slot = new Cuckoo[3];
                    c.layer2.layer[i].bslot[j].slot[0]=null;
                    c.layer2.layer[i].bslot[j].slot[1]=null;
                    c.layer2.layer[i].bslot[j].slot[2]=null;
                } else {
                    c.layer2.layer[k].bslot[j].src = 0;
                    c.layer2.layer[k].bslot[j].pt = 0;
                    c.layer2.layer[k].bslot[j].slot = new Cuckoo[3];
                    c.layer2.layer[k].bslot[j].slot[0]=null;
                    c.layer2.layer[k].bslot[j].slot[1]=null;
                    c.layer2.layer[k].bslot[j].slot[2]=null;
                }
            }
        }
    
        // Merge c1 and c2 into c (layer 2)
        for (int i = 0; i < l2 / 2; ++i) {
            for (int j = 0; j < bup; ++j) {
                if (c1.layer2.layer[i].bslot[j].isSlotEmpty()) continue;
                if (!c.layer2.layer[mmhash(c1.layer2.layer[i].bslot[j].src, s2) % (l2 * 2)].insert(c1.layer2.layer[i].bslot[j])) {
                    mq.add(c1.layer2.layer[i].bslot[j]);
                } else {
                    c.num += 1;
                }
            }
    
            for (int j = 0; j < bup; ++j) {
                if (c2.layer2.layer[i].bslot[j].isSlotEmpty()) continue;
                if (!c.layer2.layer[mmhash(c2.layer2.layer[i].bslot[j].src, s2) % (l2 * 2)].insert(c2.layer2.layer[i].bslot[j])) {
                    mq.add(c2.layer2.layer[i].bslot[j]);
                } else {
                    c.num += 1;
                }
            }
        }
    
        // Process remaining items in the queue
        while (!mq.isEmpty()) {
            if (!c.insert(mq.peek())) {
                blacklist.add(holekick);
            }
            mq.poll();
        }
    
        // In Java, no need to explicitly delete c0, c1, or c2 as memory management is handled by garbage collection.
        return c;
    }

    //public boolean insert(Edge e, int weight) {
    public void insert(int srcID, int dstID, CGRecord record){
    //    System.out.println("doing ex inserting");
        Edge e = new Edge();
        e.src=srcID;
        e.dst=dstID;
        //CGRecord weight=record;
        CGRelation weight=new CGRelation(srcID,dstID);
        relations.add(weight);
        records.add(record);
        int newRec = recordNum;
        weight.nextRec = newRec;
        recordNum++;
        CGRelation res = queryRel(srcID, dstID);
        if(res != null){
            record.nextRec = res.nextRec;
            res.nextRec = newRec;
        //    if(queryRel(srcID, dstID)==null)System.out.println("1!");
            return;
        }

        int ans = insert_query3(e, weight);
        if (ans == sure) {
        //    System.out.println("sure");
        //    if(queryRel(srcID, dstID)==null)System.out.println("2!");
            return ;
        }
        if (ans != 0) {
            if (ans == blist) {
                blacklist.get(idth).insert(e, weight);
          //      if(queryRel(srcID, dstID)==null)System.out.println("3!");
                return ;
            }
            bCuckoo ptr = link[ans - 1];
            if (layernum == 1) {
                ptr.layer1.layer[idth].bslot[bslotnum].insert(e, weight);
            //     if(ptr.layer1.layer[idth].bslot[bslotnum].query(e)==null){System.out.println("40!");
            //     for (wEdge it : sblacklist) {
            //         if (it.src == e.src && it.dst == e.dst) {
            //             System.out.println("yes");
            //         }
            //     }
            //     System.out.println(ptr.layer1.layer[idth].bslot[bslotnum].src);
            //     System.out.println(e.src);
            //     System.out.println(e.dst);
                
            // }
            //     if(queryRel(srcID, dstID)==null)System.out.println("4!");
                return ;
            } else if (layernum == 2) {
                ptr.layer2.layer[idth].bslot[bslotnum].insert(e, weight);
            //     if(ptr.layer2.layer[idth].bslot[bslotnum].query(e)==null){System.out.println("50!");
            //     for (wEdge it : sblacklist) {
            //         if (it.src == e.src && it.dst == e.dst) {
            //             System.out.println("yes");
            //         }
            //     }
            //     System.out.println(ptr.layer2.layer[idth].bslot[bslotnum].src);
            //     System.out.println(e.src);
            //     System.out.println(e.dst);
            // }
            //     if(queryRel(srcID, dstID)==null)System.out.println("5!");
                return ;
            }
        }
       // if(ans!=0)
       // System.out.println("ans: "+ans);
        // System.out.println("blist: "+blist);
        // for (bSlot it : blacklist) {
        //     if (it.src == e.src) {
        //         System.out.println("why?!");
        //     }
        // }

        if (state == -1) {
            link[0] = new bCuckoo(ll1, ll2, s1, s2);
            state = 0;
            if (!link[0].insert(e, weight)) {
                blacklist.add(holekick);
            }
            //if(queryRel(srcID, dstID)==null)System.out.println("6!");
            return ;
        }

        if (state == 0) {
            if ((link[0].num >= (link[0].len1 + link[0].len2) * bup * rate) || (blacklist.size() >= bbl)) {
                link[1] = new bCuckoo(link[0].len1 / 2, link[0].len2 / 2, s1, s2);
                state = 1;
                Queue<bSlot> qq = new LinkedList<>();
                for (bSlot it : blacklist) {
                    if (!link[1].insert(it)) {
                        qq.add(holekick);
                    }
                }
                blacklist.clear();
                while (!qq.isEmpty()) {
                    blacklist.add(qq.poll());
                }
                if (!link[1].insert(e, weight)) {
                    blacklist.add(holekick);
                }
             //   if(queryRel(srcID, dstID)==null)System.out.println("7!");
                return ;
            } else {
                if (!link[0].insert(e, weight)) {
                    blacklist.add(holekick);
                }
               // if(queryRel(srcID, dstID)==null)System.out.println("8!");
                return ;
            }
        }

        if (state == 1) {
            for (int i = state; i >= 0; --i) {
                if ((link[i].num < (link[i].len1 + link[i].len2) * bup * rate) && (blacklist.size() < bbl)) {
                    // for (bSlot it : blacklist) {
                    //     if (it.src == e.src) {
                    //         System.out.println("bhas?!");
                    //     }
                    // }
                    if (!link[i].insert(e, weight)) {
                        blacklist.add(holekick);
                    //     if(queryRel(srcID, dstID)==null){System.out.println("90!");
                    //     if(holekick.src==srcID)System.out.println("91!");
                    //     if(holekick.query(e)==null)System.out.println("92!");
                    //     for (bSlot it : blacklist) {
                    //         if (it.src == e.src) {
                    //             System.out.println("b0!");
                    //             if(it.query(e)==null)System.out.println("b1!");
                    //         }
                    //     }
                    //     System.out.println("b2!");
                    // }
                    }
                   // if(queryRel(srcID, dstID)==null)System.out.println("9! holekick.src: " + holekick.src+"  srcID: "+srcID);
                    return ;
                }
            }

            link[2] = new bCuckoo(link[1].len1, link[1].len2, s1, s2);
            state = 2;
            Queue<bSlot> qq = new LinkedList<>();
            for (bSlot it : blacklist) {
                if (!link[2].insert(it)) {
                    qq.add(holekick);
                }
            }
            blacklist.clear();
            while (!qq.isEmpty()) {
                blacklist.add(qq.poll());
            }
            if (!link[2].insert(e, weight)) {
                blacklist.add(holekick);
            }
           // if(queryRel(srcID, dstID)==null)System.out.println("10!");
            return ;
        }

        if (state == 2) {
            for (int i = state; i >= 0; --i) {
                if ((link[i].num < (link[i].len1 + link[i].len2) * bup * rate) && (blacklist.size() < bbl)) {
                    // for (bSlot it : blacklist) {
                    //     if (it.src == e.src) {
                    //         System.out.println("bhas?!");
                    //     }
                    // }
                    if (!link[i].insert(e, weight)) {
                        blacklist.add(holekick);
                    //     if(queryRel(srcID, dstID)==null){System.out.println("110!");
                    //     if(holekick.src==srcID)System.out.println("111!");
                    //     if(holekick.query(e)==null)System.out.println("112!");
                    //     for (bSlot it : blacklist) {
                    //         if (it.src == e.src) {
                    //             System.out.println("b0!");
                    //             if(it.query(e)==null)System.out.println("b1!");
                    //         }
                    //     }
                    //     System.out.println("b2!");
                    // }
                    }
                   // if(queryRel(srcID, dstID)==null)System.out.println("11!  holekick.src: " + holekick.src+"  srcID: "+srcID);
                    return ;
                }
            }

            bCuckoo cm = merge(link[0], link[1], link[2]);
            link[0] = cm;
            link[1] = new bCuckoo(link[0].len1 / 2, link[0].len2 / 2, s1, s2);
            link[2] = null;
            state = 1;
            Queue<bSlot> qq = new LinkedList<>();
            for (bSlot it : blacklist) {
                if (!link[1].insert(it)) {
                    qq.add(holekick);
                }
            }
            blacklist.clear();
            while (!qq.isEmpty()) {
                blacklist.add(qq.poll());
            }
            if (!link[1].insert(e, weight)) {
                blacklist.add(holekick);
            }
          //  if(queryRel(srcID, dstID)==null)System.out.println("12!");
            return ;
        }

      //  System.out.println("BUG!");
        return ;
    }

    public CGRelation query(Edge e) {
       
        for (int i = 0; i <= state; ++i) {
            CGRelation ans = link[i].query(e);   
            if (ans != null) return ans;
        }
    
         
        for (bSlot it : blacklist) {
            if (it.src == e.src) {
                CGRelation ans = it.query(e);  
                if (ans != null) return ans;
                return null;
            }
        }
    
        return null;
    }

    public CGRelation queryRel(int srcID, int dstID){
        Edge e= new Edge();
        e.src=srcID;
        e.dst=dstID;
        return query(e);
    }

    public int insert_query3(Edge e, CGRelation weight) {
        
        for (int i = 0; i <= state; ++i) {
            int ans = link[i].insert_query3(e, weight);  
            if (ans == sure) return sure;
            if (ans != 0) return i + 1;
        }
    
        int j = 0;
       
        for (bSlot it : blacklist) {
            if (it.src == e.src) {
                // if (it.query_insert(e, weight)!=0) {
                //     return sure;
                // }
                idth = j;
                return blist;
            }
            ++j;
        }
    
        return 0;
    }

    public int query3(Edge e) {
      
        
        for (int i = 0; i <= state; ++i) {
            int ans = link[i].query3(e);  
            if (ans == sure) return sure;
            if (ans != 0) return i + 1;
        }
    
        int j = 0;
        
        for (bSlot it : blacklist) {
            if (it.src == e.src) {
                if (it.query(e)!=null) {
                    return sure;
                }
                idth = j;
                return blist;
            }
            ++j;
        }
    
        return 0;
    }
    

        public class APHash {
            public int seed;
    
            public void setSeed(int seed){ this.seed = seed;}
    
            public int hash(String key) {
                int hash = seed;
                int n = key.length();
                for (int i = 0; i < n; i++) {
                    if ((i & 1) == 0) {
                        hash ^= ((hash << 7) ^ key.charAt(i) ^ (hash >> 3));
                    } else {
                        hash ^= (~((hash << 11) ^ key.charAt(i) ^ (hash >> 5)));
                    }
                }
                return (hash & 0x7FFFFFFF);
            }
        }
    


    public CGRelation find(int srcID, int dstID, CGRelation header){
        for(; ; header = relations.get(header.nextRel)){
            if(srcID == header.srcID && dstID == header.dstID){
                return header;
            }
            if(header.nextRel == -1)
                break;
        }
        return null;
    }

    public CGRecord queryRec(String src, String dst, int recID){
        return null;
    }

    public static class CGBucket {
        public int counter;
        public int pointer_h;
        public int pointer_t;

        public CGBucket(){
            counter = 0;
            pointer_h = -1;
            pointer_t = -1;
        }
    }

    public static class CGRelation {
        public int srcID;   ///useless
        public int dstID;
        public int nextRel;  ///useless
        public int preRel;   ///useless
        public int nextRec;
        public CGRelation(int srcID, int dstID){
            this.srcID = srcID;     ///useless
            this.dstID = dstID;
            nextRel = -1;          ///useless
            nextRec = -1;
            preRel = -1;           ///useless
        }
    }

    public static class CGRecord {
        public int relID;
        public int nextRec;

        public CGRecord(int relID){
            this.relID = relID;
            nextRec = -1;
        }
    }

    public static int CLHtoInt(char[] c){
        int res = 0;
        for(int i = 0; i < c.length; ++i){
            res += (c[i] & 0xff) << (8 * i);
        }
        return res;
    }

    public static char[] CtoLH(int n){
        char[] c = new char[4];
        c[0] = (char) (n & 0xff);
        c[1] = (char) (n >> 8 & 0xff);
        c[2] = (char) (n >> 16 & 0xff);
        c[3] = (char) (n >> 24 & 0xff);
        return c;
    }

    public static CGRecord buildCGRecord(int relationshipID){
        return new CGRecord(relationshipID);
    }



    public static CuckooGraph loadFromFile(File file){
        System.out.println("begin loadFromFile");
        return null;
    }
    public static void SaveToFile(File file, CuckooGraph cuckoo){
        System.out.println("begin SaveToFile");
    }

}


