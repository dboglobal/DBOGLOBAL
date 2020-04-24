CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1108;
	title = 110802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 13;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 781;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 14;
			prelnk = "0;";
			nolnk = 1;
			rm = 0;
			yeslnk = 254;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 6;
				iidx = 781;
			}
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "51;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "14;2;203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 201;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 60000;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "202;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = -1;
				osh_sh = 255;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 51;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 110820;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "14;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 110807;
			nolnk = 255;
			rm = 0;
			yeslnk = 14;

			CDboTSCheckObjItem
			{
				tid = 1108;
			}
			CDboTSClickObject
			{
				objidx = "611;613;614;616;617;618;619;";
				widx = 1;
			}
		}
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
	}
}

