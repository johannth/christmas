let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  didMount: _self => Analytics.(Event.track(OpenCardEvent.make())),
  render: _self =>
    <div className="Background">
      <Snow />
      <div className="App">
        <Card
          path={
            switch (Location.pathname) {
            | "/" => None
            | pathname => Some(pathname)
            }
          }
        />
      </div>
    </div>,
};