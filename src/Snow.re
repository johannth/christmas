[@bs.module] external reactClass : ReasonReact.reactClass = "react-snow-effect";

let make = (children) => ReasonReact.wrapJsForReason(~reactClass, ~props={"a": ""}, children);