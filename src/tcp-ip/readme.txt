tcp/ip protocols

=====================================================================================================================

                +-------+        +-------+            +-------+        +-------+ 
                |  APP  |        |  APP  |            |  APP  |        |  APP  |        application layer
                +-------+        +-------+            +-------+        +-------+            
                    |                |                    |                |
                    |                |                    |                |
====================|================|====================|================|=========================================
                    |                |                    |                |
                    |   +-------+    |                    |   +-------+    |            transmission layer
                    +---|  TCP  |----+                    +---|  UDP  |----+ 
                        +-------+                             +-------+     
                            |                                    | 
============================|====================================|===================================================
                            |                                    |
                            |           +-------+                | 
                            +-----------|   IP  |----------------+   +-------+          network layer
                                        +-------+--------------------|  ICMP | 
                                            |                        +-------+ 
                        +-------+           |            +-------+
                        |  ARP  |           |            | RARP  |
                        +-------+           |            +-------+
                            |               |                |
============================|===============|================|=======================================================
                            |               |                |
                            |           +-------+            |                          datalink layer
                            +-----------| ether |------------+
                                        +---|---+
                                            |
                                        ehterFrame
=====================================================================================================================



