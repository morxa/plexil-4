<?xml version="1.0" encoding="UTF-8"?>
<!-- Generated by PlexiLisp --><PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator">
   <Node NodeType="NodeList">
      <NodeId>root</NodeId>
      <VariableDeclarations>
         <DeclareVariable>
            <Name>x</Name>
            <Type>Integer</Type>
         </DeclareVariable>
      </VariableDeclarations>
      <NodeBody>
         <NodeList>
            <Node NodeType="NodeList" epx="Sequence">
               <NodeId>ep2cp_Sequence_d1e25</NodeId>
               <InvariantCondition>
                  <NOT>
                     <OR>
                        <EQInternal>
                           <NodeOutcomeVariable>
                              <NodeId>plexilisp_Name_1</NodeId>
                           </NodeOutcomeVariable>
                           <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                        </EQInternal>
                        <EQInternal>
                           <NodeOutcomeVariable>
                              <NodeId>plexilisp_Name_4</NodeId>
                           </NodeOutcomeVariable>
                           <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                        </EQInternal>
                     </OR>
                  </NOT>
               </InvariantCondition>
               <NodeBody>
                  <NodeList>
                     <Node NodeType="Assignment">
                        <NodeId>plexilisp_Name_1</NodeId>
                        <NodeBody>
                           <Assignment>
                              <IntegerVariable>x</IntegerVariable>
                              <NumericRHS>
                                 <IntegerValue>1</IntegerValue>
                              </NumericRHS>
                           </Assignment>
                        </NodeBody>
                     </Node>
                     <Node NodeType="NodeList">
                        <NodeId>plexilisp_Name_4</NodeId>
                        <VariableDeclarations>
                           <DeclareVariable>
                              <Name>y</Name>
                              <Type>Integer</Type>
                           </DeclareVariable>
                           <DeclareVariable>
                              <Name>s</Name>
                              <Type>String</Type>
                           </DeclareVariable>
                        </VariableDeclarations>
                        <StartCondition>
                           <EQInternal>
                              <NodeStateVariable>
                                 <NodeId>plexilisp_Name_1</NodeId>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                           </EQInternal>
                        </StartCondition>
                        <NodeBody>
                           <NodeList>
                              <Node NodeType="Assignment">
                                 <NodeId>plexilisp_Name_2</NodeId>
                                 <NodeBody>
                                    <Assignment>
                                       <IntegerVariable>y</IntegerVariable>
                                       <NumericRHS>
                                          <ADD>
                                             <IntegerVariable>x</IntegerVariable>
                                             <IntegerValue>4</IntegerValue>
                                          </ADD>
                                       </NumericRHS>
                                    </Assignment>
                                 </NodeBody>
                              </Node>
                              <Node NodeType="Assignment">
                                 <NodeId>plexilisp_Name_3</NodeId>
                                 <NodeBody>
                                    <Assignment>
                                       <StringVariable>s</StringVariable>
                                       <StringRHS>
                                          <StringValue>foo</StringValue>
                                       </StringRHS>
                                    </Assignment>
                                 </NodeBody>
                              </Node>
                           </NodeList>
                        </NodeBody>
                     </Node>
                  </NodeList>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>