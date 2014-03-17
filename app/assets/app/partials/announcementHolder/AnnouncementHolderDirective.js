angular.module('codepuppy').directive('announcementHolder', function() {
  return {
    restrict: 'E',
    scope: {
      'announcement': '=announcement'
    },
    templateUrl: '/assets/partials/announcementHolder/announcementHolder.html'
  };
});
