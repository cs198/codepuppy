angular.module('codepuppy').controller('AnnouncementListCtrl', ['$scope', function($scope) {
	$scope.sortByField = "date_added";
	$scope.reverse = true;
	$scope.announcements = [{date_added: '2012-08-14', subject: 'Remember to buy a textbook people', body: 'BLAH BLAH BLAH'}, {date_added: '2012-08-17', subject: 'First sections tomorrow', body: 'BLAH FIRST BLAH'}, ];
	$scope.selectedAnnouncement = null;

	$scope.announcementSelected = function(announcement) {
		$scope.selectedAnnouncement = announcement;
	};
}]);