<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator"
            FileName="Increment-test.ple">
   <GlobalDeclarations LineNo="2" ColNo="0">
      <LibraryNodeDeclaration LineNo="2" ColNo="0">
         <Name>Increment</Name>
         <Interface LineNo="2" ColNo="25">
            <In>
               <DeclareVariable LineNo="1" ColNo="25">
                  <Name>x</Name>
                  <Type>Integer</Type>
               </DeclareVariable>
            </In>
            <InOut>
               <DeclareVariable LineNo="1" ColNo="39">
                  <Name>result</Name>
                  <Type>Integer</Type>
               </DeclareVariable>
            </InOut>
         </Interface>
      </LibraryNodeDeclaration>
      <CommandDeclaration LineNo="3" ColNo="0">
         <Name>pprint</Name>
      </CommandDeclaration>
   </GlobalDeclarations>
   <Node NodeType="NodeList" epx="Sequence" LineNo="7" ColNo="2">
      <NodeId>LibraryCallTest</NodeId>
      <VariableDeclarations>
         <DeclareVariable LineNo="6" ColNo="2">
            <Name>result</Name>
            <Type>Integer</Type>
         </DeclareVariable>
      </VariableDeclarations>
      <InvariantCondition>
         <NOT>
            <OR>
               <AND>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">LibraryCall__0</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="child">LibraryCall__0</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </AND>
               <AND>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">COMMAND__1</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="child">COMMAND__1</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </AND>
               <AND>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">LibraryCall__2</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="child">LibraryCall__2</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </AND>
               <AND>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">COMMAND__3</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="child">COMMAND__3</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </AND>
            </OR>
         </NOT>
      </InvariantCondition>
      <NodeBody>
         <NodeList>
            <Node NodeType="LibraryNodeCall">
               <NodeId>LibraryCall__0</NodeId>
               <NodeBody>
                  <LibraryNodeCall>
                     <NodeId>Increment</NodeId>
                     <Alias>
                        <NodeParameter>x</NodeParameter>
                        <IntegerValue>1</IntegerValue>
                     </Alias>
                     <Alias>
                        <NodeParameter>result</NodeParameter>
                        <IntegerVariable>result</IntegerVariable>
                     </Alias>
                  </LibraryNodeCall>
               </NodeBody>
            </Node>
            <Node NodeType="Command" LineNo="8" ColNo="2">
               <NodeId>COMMAND__1</NodeId>
               <StartCondition>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="sibling">LibraryCall__0</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </StartCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>pprint</StringValue>
                     </Name>
                     <Arguments LineNo="9" ColNo="10">
                        <StringValue>Increment(1) =</StringValue>
                        <IntegerVariable>result</IntegerVariable>
                     </Arguments>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="LibraryNodeCall">
               <NodeId>LibraryCall__2</NodeId>
               <StartCondition>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="sibling">COMMAND__1</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </StartCondition>
               <NodeBody>
                  <LibraryNodeCall>
                     <NodeId>Increment</NodeId>
                     <Alias>
                        <NodeParameter>x</NodeParameter>
                        <IntegerVariable>result</IntegerVariable>
                     </Alias>
                     <Alias>
                        <NodeParameter>result</NodeParameter>
                        <IntegerVariable>result</IntegerVariable>
                     </Alias>
                  </LibraryNodeCall>
               </NodeBody>
            </Node>
            <Node NodeType="Command" LineNo="10" ColNo="2">
               <NodeId>COMMAND__3</NodeId>
               <StartCondition>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="sibling">LibraryCall__2</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </StartCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>pprint</StringValue>
                     </Name>
                     <Arguments LineNo="11" ColNo="10">
                        <StringValue>Increment(2) =</StringValue>
                        <IntegerVariable>result</IntegerVariable>
                     </Arguments>
                  </Command>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>