/* [@bs.module] external frontImage: string = "./front-transparent.png";

   [@bs.module] external frontImagePaper: string = "./0-front.jpeg";

   [@bs.module] external frontBackImagePaper: string = "./1-empty-page.jpeg";

   [@bs.module] external backImagePaper: string = "./2-text-page.jpeg"; */

type openState =
  | Initial
  | Open
  | Closed;

type state = {
  openState,
  lines: array(string),
};

type action =
  | Open
  | Close
  | DidLoadContent(array(string));

let classNameFromState = (state: openState) =>
  switch (state) {
  | Initial => "CardInitial"
  | Open => "CardOpen"
  | Closed => "CardClosed"
  };

module Fetch = {
  exception ResponseError(unit);
  type response;
  type body;
  [@bs.get] external ok: response => bool = "ok";
  [@bs.get] external body: response => body = "body";
  [@bs.send] external text: response => Js.Promise.t(string) = "text";
  [@bs.val] external fetch: string => Js.Promise.t(response) = "fetch";
};

[@bs.send.pipe: string] external split: string => array(string) = "split";

let component = ReasonReact.reducerComponent("Card");

let make = (~path: option(string), _children) => {
  ...component,
  initialState: () => {
    lines: [|
      {j|Kæru vinir!|j},
      {j|Gleðileg jól og farsælt komandi ár!|j},
      {j|Við þökkum kærlega fyrir árið sem líður í aldana skaut.|j},
      {j|Við vonum að næsta ár verði fullt af ævintýrum. Það er alltaf heitt á könnunni á Bragagötu.|j},
      {j|Ástarkveðjur,|j},
      {j|Stefanía, Jói & Súkkan|j},
    |],
    openState: Initial,
  },
  reducer: (action, state) =>
    switch (state.openState, action) {
    | (Initial, Open)
    | (Initial, Close)
    | (Closed, Open) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, openState: Open},
        (
          _self =>
            Analytics.(
              Event.track(FlipCardEvent.(make(Side.Front, Side.Open)))
            )
        ),
      )
    | (Open, Close) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, openState: Closed},
        (
          _self =>
            Analytics.(
              Event.track(FlipCardEvent.(make(Side.Open, Side.Front)))
            )
        ),
      )
    | (Closed, Close)
    | (Open, Open) => ReasonReact.NoUpdate
    | (_, DidLoadContent(lines)) => ReasonReact.Update({...state, lines})
    },
  didMount: self =>
    switch (path) {
    | Some(path) =>
      Fetch.(
        fetch({j|content$path|j})
        |> Js.Promise.then_(response =>
             Fetch.ok(response) ?
               response |> text : Js.Promise.reject(ResponseError())
           )
        |> Js.Promise.then_(text =>
             Js.String.indexOf("<html", text) != (-1) ?
               Js.Promise.reject(ResponseError()) :
               {
                 self.send(DidLoadContent(Js.String.split("\n", text)));
                 Js.Promise.resolve();
               }
           )
        |> ignore
      )
    | None => ()
    },
  render: self =>
    <div className={"Card " ++ classNameFromState(self.state.openState)}>
      <div
        className="CardFront CardFace"
        /* style={
             ReactDOMRe.Style.make(
               ~backgroundImage={j|url($frontImagePaper)|j},
               (),
             )
           } */
        onClick={_ => self.send(Open)}>
        <h1> {ReasonReact.string({js|Gleðileg jól!|js})} </h1>
        <img className="CardFrontImage" />
        <h2> {ReasonReact.string({js|Vúhú & jibbí!|js})} </h2>
      </div>
      <div
        className="CardFrontBack CardFace"
        /* style={
             ReactDOMRe.Style.make(
                  ~backgroundImage={j|url($frontBackImagePaper)|j},
                  (),
                )
           } */
        onClick={_ => self.send(Close)}
      />
      <div
        className="CardBack CardFace"
        /* style={
             ReactDOMRe.Style.make(
               ~backgroundImage={j|url($backImagePaper)|j},
               (),
             )
           } */
        onClick={_ => self.send(Close)}>
        <div>
          ...{
               self.state.lines
               |> Array.mapi((i, line) =>
                    <p key={string_of_int(i)} className="Paragraph">
                      {ReasonReact.string(line)}
                    </p>
                  )
             }
        </div>
        <div className="CardBackShadow" />
      </div>
    </div>,
};