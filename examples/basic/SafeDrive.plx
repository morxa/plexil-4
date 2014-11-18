<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator"
            FileName="SafeDrive.ple">
   <GlobalDeclarations LineNo="2" ColNo="0">
      <CommandDeclaration LineNo="2" ColNo="0">
         <Name>Drive</Name>
         <Parameter>
            <Type>Integer</Type>
         </Parameter>
      </CommandDeclaration>
      <CommandDeclaration LineNo="3" ColNo="0">
         <Name>TakePicture</Name>
      </CommandDeclaration>
      <CommandDeclaration LineNo="4" ColNo="0">
         <Name>pprint</Name>
      </CommandDeclaration>
      <StateDeclaration LineNo="5" ColNo="8">
         <Name>WheelStuck</Name>
         <Return>
            <Name>_return_0</Name>
            <Type>Boolean</Type>
         </Return>
      </StateDeclaration>
   </GlobalDeclarations>
   <Node NodeType="NodeList" epx="Sequence" LineNo="9" ColNo="2">
      <NodeId>SafeDrive</NodeId>
      <VariableDeclarations>
         <DeclareVariable LineNo="8" ColNo="2">
            <Name>pictures</Name>
            <Type>Integer</Type>
            <InitialValue>
               <IntegerValue>0</IntegerValue>
            </InitialValue>
         </DeclareVariable>
      </VariableDeclarations>
      <EndCondition>
         <OR>
            <LookupOnChange>
               <Name>
                  <StringValue>WheelStuck</StringValue>
               </Name>
            </LookupOnChange>
            <EQNumeric>
               <IntegerVariable>pictures</IntegerVariable>
               <IntegerValue>10</IntegerValue>
            </EQNumeric>
         </OR>
      </EndCondition>
      <InvariantCondition>
         <NOT>
            <EQInternal>
               <NodeOutcomeVariable>
                  <NodeId>while__0</NodeId>
               </NodeOutcomeVariable>
               <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
            </EQInternal>
         </NOT>
      </InvariantCondition>
      <NodeBody>
         <NodeList>
            <Node NodeType="NodeList" epx="While" LineNo="12" ColNo="2">
               <NodeId>while__0</NodeId>
               <RepeatCondition>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">ep2cp_WhileTest</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>SUCCESS</NodeOutcomeValue>
                  </EQInternal>
               </RepeatCondition>
               <NodeBody>
                  <NodeList>
                     <Node NodeType="Empty" epx="Condition">
                        <NodeId>ep2cp_WhileTest</NodeId>
                        <PostCondition>
                           <NOT>
                              <LookupOnChange>
                                 <Name>
                                    <StringValue>WheelStuck</StringValue>
                                 </Name>
                              </LookupOnChange>
                           </NOT>
                        </PostCondition>
                     </Node>
                     <Node NodeType="NodeList" epx="Action" LineNo="14" ColNo="4">
                        <NodeId>BLOCK__1</NodeId>
                        <InvariantCondition>
                           <NOT>
                              <OR>
                                 <EQInternal>
                                    <NodeOutcomeVariable>
                                       <NodeId>OneMeter</NodeId>
                                    </NodeOutcomeVariable>
                                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                 </EQInternal>
                                 <EQInternal>
                                    <NodeOutcomeVariable>
                                       <NodeId>TakePic</NodeId>
                                    </NodeOutcomeVariable>
                                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                 </EQInternal>
                                 <EQInternal>
                                    <NodeOutcomeVariable>
                                       <NodeId>Counter</NodeId>
                                    </NodeOutcomeVariable>
                                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                 </EQInternal>
                                 <EQInternal>
                                    <NodeOutcomeVariable>
                                       <NodeId>Print</NodeId>
                                    </NodeOutcomeVariable>
                                    <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                 </EQInternal>
                              </OR>
                           </NOT>
                        </InvariantCondition>
                        <StartCondition>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>SUCCESS</NodeOutcomeValue>
                           </EQInternal>
                        </StartCondition>
                        <SkipCondition>
                           <AND>
                              <EQInternal>
                                 <NodeStateVariable>
                                    <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                                 </NodeStateVariable>
                                 <NodeStateValue>FINISHED</NodeStateValue>
                              </EQInternal>
                              <EQInternal>
                                 <NodeFailureVariable>
                                    <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                                 </NodeFailureVariable>
                                 <NodeFailureValue>POST_CONDITION_FAILED</NodeFailureValue>
                              </EQInternal>
                           </AND>
                        </SkipCondition>
                        <NodeBody>
                           <NodeList>
                              <Node NodeType="Command" LineNo="13" ColNo="16">
                                 <NodeId>OneMeter</NodeId>
                                 <NodeBody>
                                    <Command>
                                       <Name>
                                          <StringValue>Drive</StringValue>
                                       </Name>
                                       <Arguments LineNo="14" ColNo="22">
                                          <IntegerValue>1</IntegerValue>
                                       </Arguments>
                                    </Command>
                                 </NodeBody>
                              </Node>
                              <Node NodeType="Command" LineNo="16" ColNo="6">
                                 <NodeId>TakePic</NodeId>
                                 <StartCondition>
                                    <AND>
                                       <EQInternal>
                                          <NodeStateVariable>
                                             <NodeId>OneMeter</NodeId>
                                          </NodeStateVariable>
                                          <NodeStateValue>FINISHED</NodeStateValue>
                                       </EQInternal>
                                       <LT>
                                          <IntegerVariable>pictures</IntegerVariable>
                                          <IntegerValue>10</IntegerValue>
                                       </LT>
                                    </AND>
                                 </StartCondition>
                                 <NodeBody>
                                    <Command>
                                       <Name>
                                          <StringValue>TakePicture</StringValue>
                                       </Name>
                                    </Command>
                                 </NodeBody>
                              </Node>
                              <Node NodeType="Assignment" LineNo="21" ColNo="6">
                                 <NodeId>Counter</NodeId>
                                 <StartCondition>
                                    <EQInternal>
                                       <NodeStateVariable>
                                          <NodeId>TakePic</NodeId>
                                       </NodeStateVariable>
                                       <NodeStateValue>FINISHED</NodeStateValue>
                                    </EQInternal>
                                 </StartCondition>
                                 <PreCondition>
                                    <LT>
                                       <IntegerVariable>pictures</IntegerVariable>
                                       <IntegerValue>10</IntegerValue>
                                    </LT>
                                 </PreCondition>
                                 <NodeBody>
                                    <Assignment>
                                       <IntegerVariable>pictures</IntegerVariable>
                                       <NumericRHS>
                                          <ADD LineNo="21" ColNo="26">
                                             <IntegerVariable>pictures</IntegerVariable>
                                             <IntegerValue>1</IntegerValue>
                                          </ADD>
                                       </NumericRHS>
                                    </Assignment>
                                 </NodeBody>
                              </Node>
                              <Node NodeType="Command" LineNo="22" ColNo="13">
                                 <NodeId>Print</NodeId>
                                 <StartCondition>
                                    <EQInternal>
                                       <NodeStateVariable>
                                          <NodeId>Counter</NodeId>
                                       </NodeStateVariable>
                                       <NodeStateValue>FINISHED</NodeStateValue>
                                    </EQInternal>
                                 </StartCondition>
                                 <NodeBody>
                                    <Command>
                                       <Name>
                                          <StringValue>pprint</StringValue>
                                       </Name>
                                       <Arguments LineNo="23" ColNo="21">
                                          <StringValue>Pictures taken:</StringValue>
                                          <IntegerVariable>pictures</IntegerVariable>
                                       </Arguments>
                                    </Command>
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