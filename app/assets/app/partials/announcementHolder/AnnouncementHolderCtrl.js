angular.module('codepuppy').controller('AnnouncementHolderCtrl', ['$scope', function($scope) {
	var createTime = moment($scope.announcement.created_at);
	$scope.createdAtFormatted = createTime.format('MMM DD, YYYY, hh:mm A');
}]);