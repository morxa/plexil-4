<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator"
            FileName="lib2.ple">
   <GlobalDeclarations LineNo="2" ColNo="0">
      <LibraryNodeDeclaration LineNo="2" ColNo="0">
         <Name>LibTest1</Name>
         <Interface LineNo="2" ColNo="24">
            <In>
               <DeclareVariable LineNo="1" ColNo="24">
                  <Name>lb</Name>
                  <Type>Boolean</Type>
               </DeclareVariable>
               <DeclareVariable LineNo="2" ColNo="24">
                  <Name>li</Name>
                  <Type>Integer</Type>
               </DeclareVariable>
               <DeclareVariable LineNo="3" ColNo="24">
                  <Name>lr</Name>
                  <Type>Real</Type>
               </DeclareVariable>
               <DeclareVariable LineNo="4" ColNo="24">
                  <Name>ls</Name>
                  <Type>String</Type>
               </DeclareVariable>
            </In>
         </Interface>
      </LibraryNodeDeclaration>
      <LibraryNodeDeclaration LineNo="6" ColNo="0">
         <Name>LibTest2</Name>
         <Interface LineNo="6" ColNo="24">
            <In>
               <DeclareVariable LineNo="5" ColNo="24">
                  <Name>y</Name>
                  <Type>Real</Type>
               </DeclareVariable>
               <DeclareVariable LineNo="5" ColNo="35">
                  <Name>z</Name>
                  <Type>Real</Type>
               </DeclareVariable>
            </In>
         </Interface>
      </LibraryNodeDeclaration>
      <CommandDeclaration LineNo="7" ColNo="0">
         <Name>bar</Name>
         <Parameter>
            <Type>Real</Type>
         </Parameter>
         <Parameter>
            <Type>Real</Type>
         </Parameter>
      </CommandDeclaration>
   </GlobalDeclarations>
   <Node NodeType="NodeList" epx="Sequence" LineNo="11" ColNo="2">
      <NodeId>LibTest2</NodeId>
      <Interface>
         <In>
            <DeclareVariable LineNo="10" ColNo="5">
               <Name>y</Name>
               <Type>Real</Type>
            </DeclareVariable>
            <DeclareVariable LineNo="11" ColNo="5">
               <Name>z</Name>
               <Type>Real</Type>
            </DeclareVariable>
         </In>
      </Interface>
      <InvariantCondition>
         <NOT>
            <OR>
               <AND>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">BarCall2</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="child">BarCall2</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </AND>
               <AND>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">Call2LibTest1</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                  </EQInternal>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="child">Call2LibTest1</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </AND>
            </OR>
         </NOT>
      </InvariantCondition>
      <NodeBody>
         <NodeList>
            <Node NodeType="Command" LineNo="13" ColNo="12">
               <NodeId>BarCall2</NodeId>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>bar</StringValue>
                     </Name>
                     <Arguments LineNo="14" ColNo="16">
                        <RealVariable>y</RealVariable>
                        <RealVariable>z</RealVariable>
                     </Arguments>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="LibraryNodeCall">
               <NodeId>Call2LibTest1</NodeId>
               <StartCondition>
                  <EQInternal>
                     <NodeStateVariable>
                        <NodeRef dir="sibling">BarCall2</NodeRef>
                     </NodeStateVariable>
                     <NodeStateValue>FINISHED</NodeStateValue>
                  </EQInternal>
               </StartCondition>
               <NodeBody>
                  <LibraryNodeCall>
                     <NodeId>LibTest1</NodeId>
                     <Alias>
                        <NodeParameter>lb</NodeParameter>
                        <BooleanValue>false</BooleanValue>
                     </Alias>
                     <Alias>
                        <NodeParameter>li</NodeParameter>
                        <IntegerValue>123</IntegerValue>
                     </Alias>
                     <Alias>
                        <NodeParameter>lr</NodeParameter>
                        <RealVariable>y</RealVariable>
                     </Alias>
                     <Alias>
                        <NodeParameter>ls</NodeParameter>
                        <StringValue>what!</StringValue>
                     </Alias>
                  </LibraryNodeCall>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>