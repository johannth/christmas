[%bs.raw {|require('./app.css')|}];

module Location = {
  [@bs.scope ("window", "location")] [@bs.val] external pathname : string = "pathname";
};

let component = ReasonReact.statelessComponent("App");

let make = (_children) => {
  ...component,
  render: (_self) =>
    <div className="Background">
      <Snow />
      <div className="App"> <Card path=Location.pathname /> </div>
    </div>
};