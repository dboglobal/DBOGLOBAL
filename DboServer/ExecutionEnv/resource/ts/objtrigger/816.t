CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 816;
	title = 81602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 81607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 816;
			}
			CDboTSClickObject
			{
				objidx = "7;8;9;10;11;12;13;14;";
				widx = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 11;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 2;
			cid = 11;
			cid1 = 21;
			prelnk = "3;";
			bid0 = 1;
			cid0 = 12;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.800000";
				sbi = 1;
				taid = 1;
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
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
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
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 51;
			prelnk = "13;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 81618;
			}
		}
		CDboTSContGAct
		{
			cid = 21;
			elnk = 255;
			nextlnk = 22;
			prelnk = "11;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 201;
			prelnk = "14;22;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
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
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 14;
			prelnk = "12;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 581;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 51;
			prelnk = "21;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 81619;
			}
		}
	}
}

