CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 2;
	rq = 0;
	tid = 469;
	title = 46902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 254;
			prelnk = "6;";

			CDboTSActItem
			{
				iidx0 = 99067;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "100;";
			nolnk = 254;
			rm = 0;
			yeslnk = 7;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99067;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 46934;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActItem
			{
				iidx0 = 99067;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 86;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "6;7;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 46907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4372207;";
			}
			CDboTSCheckClrQst
			{
				and = "468;";
				flink = 1;
				flinknextqid = "470;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 46909;
				ctype = 1;
				idx = 4372207;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 46908;
				area = 46901;
				goal = 46904;
				m0fz = "-3166.459961";
				m0widx = 1;
				scitem = -1;
				sort = 46905;
				stype = 2;
				m0fy = "-82.959000";
				m0pat = 0;
				grade = 132203;
				m0fx = "6588.080078";
				m0ttip = 46915;
				rwd = 100;
				taid = 1;
				title = 46902;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 46908;
			gtype = 2;
			oklnk = 2;
			area = 46901;
			goal = 46904;
			sort = 46905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 46902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 46914;
			nextlnk = 6;
			rwdexp = 0;
			rwdtbl = 46901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372207;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 3;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99067;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActItem
			{
				iidx0 = 99067;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 3;
			prelnk = "2;1;";
			type = 0;
		}
	}
}

