CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 737;
	title = 73702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 73708;
				gtype = 2;
				area = 73701;
				goal = 73704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 73705;
				stype = 2;
				taid = 1;
				title = 73702;
			}
			CDboTSActNPCConv
			{
				conv = 73709;
				ctype = 1;
				idx = 7131109;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				dt2 = 73725;
				otype2 = 1;
				widx1 = 1;
				iidx0 = 554;
				otype1 = 1;
				widx2 = 1;
				dt0 = 73721;
				esctype = 0;
				iidx1 = 555;
				it1 = 73722;
				otype0 = 1;
				tidx2 = 3181201;
				dt1 = 73723;
				etype = 0;
				iidx2 = 555;
				it0 = 73720;
				taid = 3;
				tidx1 = 3173103;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 2972107;
				ectype = -1;
				it2 = 73724;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 73714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 73701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131109;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 73708;
			gtype = 2;
			oklnk = 2;
			area = 73701;
			goal = 73704;
			sort = 73705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 73702;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 73707;
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
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "7131109;";
			}
			CDboTSCheckClrQst
			{
				and = "736;";
				flink = 0;
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 555;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 554;
				iprob1 = "1.000000";
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
	}
}

