CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 34;
	title = 3402;

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
				npcidx = "2951101;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "2951101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
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
			stdiag = 3407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 6;
			}
			CDboTSClickNPC
			{
				npcidx = "5061201;";
			}
			CDboTSCheckClrQst
			{
				and = "33;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92004;
				cnt1 = 0;
				eitype = 2;
				idx0 = 7;
				cnt0 = 1;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 3;
			}
			CDboTSActRegQInfo
			{
				cont = 3408;
				gtype = 1;
				area = 3401;
				goal = 3404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 3405;
				stype = 1;
				taid = 1;
				title = 3402;
			}
			CDboTSActNPCConv
			{
				conv = 3409;
				ctype = 1;
				idx = 5061201;
				taid = 2;
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
			desc = 3414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 3401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "101;4;";
			usetbl = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 3408;
			gtype = 1;
			oklnk = 2;
			area = 3401;
			goal = 3404;
			sort = 3405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 3402;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

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
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 7;
				taid = 2;
				type = 1;
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
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 7;
			}
		}
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
				iidx0 = 7;
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
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
	}
}

