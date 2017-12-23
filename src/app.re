[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: (_self) =>
    <div className="Background">
      <Snow />
      <div className="App"> <Card message="This is a greeting" /> </div>
    </div>
};