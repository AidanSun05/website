<template>
  <div v-show="tabs[id] === index" class="tab">
    <slot></slot>
  </div>
</template>

<script setup lang="ts">
import { useStore } from "@nanostores/vue";
import { $tabs } from "@common/store/tabs";

withDefaults(
  defineProps<{
    id?: string;
    index: number;
  }>(),
  {
    id: "default"
  }
);

const tabs = useStore($tabs);
</script>

<style>
.tab {
  padding: 6px;
  border: 1px solid #ccc;
}
</style>
