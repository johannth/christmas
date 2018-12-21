exception ResponseError(unit);
type response;
type body;
[@bs.get] external ok: response => bool = "ok";
[@bs.get] external body: response => body = "body";
[@bs.send] external text: response => Js.Promise.t(string) = "text";
[@bs.val] external fetch: string => Js.Promise.t(response) = "fetch";