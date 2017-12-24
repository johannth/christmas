let cardOwner = () =>
  switch Location.pathname {
  | "/" => "default"
  | pathname => pathname
  };

module Event = {
  type t('a) = {
    name: string,
    properties: Js.t(({..} as 'a))
  };
  let track = (event: t('a)) =>
    Amplitude.(getInstance() |> logEvent(event.name, event.properties));
};

module OpenCardEvent = {
  open Event;
  let name = "Open Card";
  let make = () : Event.t('a) => {name, properties: {"Card Owner": cardOwner()}};
};

module FlipCardEvent = {
  open Event;
  let name = "Flip Card";
  module Side = {
    type t =
      | Front
      | Open;
    let toString = (side) =>
      switch side {
      | Front => "Front"
      | Open => "Open"
      };
  };
  let make = (from: Side.t, to_: Side.t) : Event.t('a) => {
    name,
    properties: {"Card Owner": cardOwner(), "From": Side.toString(from), "To": Side.toString(to_)}
  };
};