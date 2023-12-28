<template>
  <div class="tab-container">
    <div class="tab-bar">
      <button
        v-for="[i, title] of titles.entries()"
        :key="i"
        :class="{ tabOpen: tabs[id] === i }"
        @click="setTab(id, i)"
      >
        {{ title }}
      </button>
    </div>
    <slot></slot>
  </div>
</template>

<script setup lang="ts">
import { useStore } from "@nanostores/vue";
import { $tabs } from "@common/store/tabs";
import { setTab } from "@common/store/tabs";

const props = withDefaults(
  defineProps<{
    id?: string;
    titles: string[];
  }>(),
  {
    id: "default"
  }
);

const tabs = useStore($tabs);
setTab(props.id, 0);
</script>

<style>
.tab-container {
  margin: 10px 0;
}

.tab-bar {
  display: flex;
  flex-direction: row;
  flex-wrap: wrap;

  button {
    background-color: inherit;
    border: none;
    cursor: pointer;
    padding: 8px;
    transition: 0.2s;

    &:hover {
      background-color: #ddd;
    }

    &.tabOpen {
      background-color: #ccc;
    }
  }
}
</style>
