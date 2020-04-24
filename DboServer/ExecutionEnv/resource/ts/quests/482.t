CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 482;
	title = 48202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 101;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "5061204;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "5061204;";
			}
			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 698;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 698;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 48207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 13;
			}
			CDboTSClickNPC
			{
				npcidx = "5041147;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 101;
			cid = 100;
			rwdzeny = 0;
			desc = 48214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 48201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "101;4;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 48208;
				gtype = 2;
				area = 48201;
				goal = 48204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 48205;
				stype = 1;
				taid = 1;
				title = 48202;
			}
			CDboTSActNPCConv
			{
				conv = 48209;
				ctype = 1;
				idx = 5041147;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92015;
				cnt1 = 0;
				eitype = 2;
				idx0 = 698;
				cnt0 = 1;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 3;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 48208;
			gtype = 2;
			oklnk = 2;
			area = 48201;
			goal = 48204;
			sort = 48205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 48202;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 698;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 698;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

