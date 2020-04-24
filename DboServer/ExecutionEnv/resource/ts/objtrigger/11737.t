CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11737;
	title = 1173702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 3;
				oidx = 18;
				osh_sh = 255;
				widx = 197000;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 23630;
				tblidx = 12319;
				stradius = 0;
				stype = 2;
				taid = 2;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 18;
				osh_sh = 255;
				widx = 197000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;7;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActDoSkill
			{
				idx = 2425;
				taid = 2;
			}
			CDboTSActTObjFriendly
			{
				flgs = 3;
				taid = 205;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10000;
				time = 3000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1173707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "18;";
				widx = 197000;
			}
		}
	}
}

