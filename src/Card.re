[%bs.raw {|require('./Card.css')|}];

[@bs.module] external frontImage : string = "./front-transparent.png";

[@bs.module] external frontImagePaper : string = "./0-front.jpeg";

[@bs.module] external frontBackImagePaper : string = "./1-empty-page.jpeg";

[@bs.module] external backImagePaper : string = "./2-text-page.jpeg";

type openState =
  | Initial
  | Open
  | Closed;

type state = {
  openState,
  lines: option(list(string))
};

type action =
  | Open
  | Close
  | DidLoadContent(list(string));

let classNameFromState = (state: openState) =>
  switch state {
  | Initial => "CardInitial"
  | Open => "CardOpen"
  | Closed => "CardClosed"
  };

module Fetch = {
  type response;
  type body;
  [@bs.get] external body : response => body = "body";
  [@bs.send] external text : response => Js.Promise.t(string) = "text";
  [@bs.val] external fetch : string => Js.Promise.t(response) = "fetch";
};

let component = ReasonReact.reducerComponent("Card");

let make = (~path, _children) => {
  ...component,
  initialState: () => {lines: None, openState: Initial},
  reducer: (action, state) =>
    switch (state.openState, action) {
    | (Initial, Open)
    | (Initial, Close)
    | (Closed, Open) => ReasonReact.Update({...state, openState: Open})
    | (Open, Close) => ReasonReact.Update({...state, openState: Closed})
    | (Closed, Close)
    | (Open, Open) => ReasonReact.NoUpdate
    | (_, DidLoadContent(lines)) => ReasonReact.Update({...state, lines: Some(lines)})
    },
  didMount: (self) => {
    Fetch.(
      fetch({j|content/$path|j})
      |> Js.Promise.then_((response) => response |> text)
      |> Js.Promise.then_(
           (text) => {
             self.reduce(() => DidLoadContent([text]), ());
             Js.Promise.resolve()
           }
         )
      |> ignore
    );
    ReasonReact.NoUpdate
  },
  render: (self) =>
    <div className=("Card " ++ classNameFromState(self.state.openState))>
      <div
        className="CardFront CardFace"
        style=(ReactDOMRe.Style.make(~backgroundImage={j|url($frontImagePaper)|j}, ()))
        onClick=(self.reduce((_event) => Open))>
        <img className="CardFrontImage" src=frontImage />
      </div>
      <div
        className="CardFrontBack CardFace"
        style=(ReactDOMRe.Style.make(~backgroundImage={j|url($frontBackImagePaper)|j}, ()))
        onClick=(self.reduce((_event) => Close))
      />
      <div
        className="CardBack CardFace"
        style=(ReactDOMRe.Style.make(~backgroundImage={j|url($backImagePaper)|j}, ()))
        onClick=(self.reduce((_event) => Close))>
        <div>
          (
            switch self.state.lines {
            | Some(lines) =>
              ReasonReact.arrayToElement(
                lines
                |> Array.of_list
                |> Array.mapi(
                     (i, line) =>
                       <p key=(string_of_int(i))> (ReasonReact.stringToElement(line)) </p>
                   )
              )
            | None => ReasonReact.nullElement
            }
          )
        </div>
        <div className="CardBackShadow" />
      </div>
    </div>
};