[%bs.raw {|require('./Card.css')|}];

[@bs.module] external frontImage : string = "./front.jpg";
[@bs.module] external frontImagePaper : string = "./0-front.jpeg";
[@bs.module] external frontBackImagePaper : string = "./1-empty-page.jpeg";
[@bs.module] external backImagePaper : string = "./2-text-page.jpeg";

type state =
  | Initial
  | Open
  | Closed;

type action =
  | Open
  | Close;

let classNameFromState = (state: state) => {
  switch state {
  | Initial => "CardInitial"
  | Open => "CardOpen"
  | Closed => "CardClosed"
  }
};

let component = ReasonReact.reducerComponent("Card");

let make = (~messageRenderCallback, _children) => {
  ...component,
  initialState: () => Initial,
  reducer: (action, state) => {
    switch (state, action) {
    | (Initial, Open) 
    | (Initial, Close)
    | (Closed, Open) => ReasonReact.Update(Open: state)
    | (Open, Close) => ReasonReact.Update(Closed: state)
    | (Closed, Close) 
    | (Open, Open) => ReasonReact.NoUpdate
    }
  },
  render: (self) =>
    <div className=("Card " ++ classNameFromState(self.state))>
      <div className="CardFront CardFace" style=(
        ReactDOMRe.Style.make(~backgroundImage={j|url($frontImagePaper)|j}, ())
      ) onClick=(self.reduce((_event) => Open))>
        <img className="CardFrontImage" src=frontImage />
      </div>
      <div className="CardFrontBack CardFace" style=(
        ReactDOMRe.Style.make(~backgroundImage={j|url($frontBackImagePaper)|j}, ())) onClick=(self.reduce((_event) => Close)) />
      <div className="CardBack CardFace" style=(
        ReactDOMRe.Style.make(~backgroundImage={j|url($backImagePaper)|j}, ())) onClick=(self.reduce((_event) => Close))>
        <div> (messageRenderCallback()) </div>
        <div className="CardBackShadow" />
      </div>
    </div>
};