angular.module('codepuppy').directive('navigationSelector', function() {
  return {
    restrict: 'E',
    scope: {
      'title': '=title',
      'elements': '=elements', // Array of hashes with :name, :url
      'selected': '=selected'
    },
    templateUrl: '/assets/partials/navigationSelector/navigationSelector.html'
  };
});
