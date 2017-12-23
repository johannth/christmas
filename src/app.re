[%bs.raw {|require('./app.css')|}];

type state = {messageRenderCallback: unit => ReasonReact.reactElement};

type action =
  | NoOp;

[@bs.scope ("window", "location")] [@bs.val] external pathname : string = "pathname";

let routeToCorrectCard = () => {
  Js.log2("pathname", pathname);
  switch pathname {
  | "/kristjan-og-dana" => (
      () => <div> (ReasonReact.stringToElement({js|Kristján og dana|js})) </div>
    )
  | _ => (() => <div> (ReasonReact.stringToElement("Allir")) </div>)
  }
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  ...component,
  initialState: () => {messageRenderCallback: routeToCorrectCard()},
  reducer: (_action: action, _state: state) => ReasonReact.NoUpdate,
  render: (self) =>
    <div className="Background">
      <Snow />
      <div className="App"> <Card messageRenderCallback=self.state.messageRenderCallback /> </div>
    </div>
};