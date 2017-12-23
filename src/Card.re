[%bs.raw {|require('./Card.css')|}];

[@bs.module] external frontImage : string = "./front.jpg";

type state =
  | Open
  | Closed;

type action =
  | Open
  | Close;

let classNameFromState = (state: state) => {
  switch state {
  | Open => "CardOpen"
  | Closed => "CardClosed"
  }
};

let component = ReasonReact.reducerComponent("Card");

let make = (~message, _children) => {
  ...component,
  initialState: () => Closed,
  reducer: (action, state) => {
    switch (state, action) {
    | (Closed, Open) => ReasonReact.Update(Open: state)
    | (Open, Close) => ReasonReact.Update(Closed: state)
    }
  },
  render: (self) =>
    <div className=("Card " ++ classNameFromState(self.state))>
      <div className="CardFront CardFace" onClick=(self.reduce((_event) => Open))>
        <img className="CardFrontImage" src=frontImage />
      </div>
      <div className="CardFrontBack CardFace" onClick=(self.reduce((_event) => Close)) />
      <div className="CardBack CardFace" onClick=(self.reduce((_event) => Close))>
        <p> (ReasonReact.stringToElement(message)) </p>
      </div>
    </div>
};