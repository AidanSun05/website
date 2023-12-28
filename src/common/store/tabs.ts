import { map } from "nanostores";

export const $tabs = map<Record<string, number>>();

export function setTab(id: string, currentTab: number) {
  $tabs.setKey(id, currentTab);
}
