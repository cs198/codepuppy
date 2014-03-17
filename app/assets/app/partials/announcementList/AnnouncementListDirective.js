angular.module('codepuppy').directive('announcementList', function() {
  return {
    restrict: 'E',
    scope: {
		'addPermissions': '=addpermissions'
    },
    templateUrl: '/assets/partials/announcementList/announcementList.html'
  };
});
