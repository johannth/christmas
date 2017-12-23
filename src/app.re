[%bs.raw {|require('./app.css')|}];

[@bs.module] external frontImage : string = "./front.jpg";

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: (_self) =>
    <div className="Background">
      <Snow />
      <div className="App">
        <div className="Card">
          <div className="CardFront CardFace">
            <img className="CardFrontImage" src=frontImage />
          </div>
          <div className="CardFrontBack CardFace" />
          <div className="CardBack CardFace">
            <p> (ReasonReact.stringToElement("This is back")) </p>
          </div>
        </div>
      </div>
    </div>
};