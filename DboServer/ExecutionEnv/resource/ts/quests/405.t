CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 405;
	title = 40502;

	CNtlTSGroup
	{
		gid = 0;

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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 40514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 40501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4152110;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 40507;
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
				minlvl = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4152110;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "412;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 40509;
				ctype = 1;
				idx = 4152110;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 3;
				idx0 = 633;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 634;
				taid = 3;
				esctype = 0;
				idx2 = 635;
			}
			CDboTSActRegQInfo
			{
				cont = 40508;
				m1fx = "5780.339844";
				m2fz = "-3989.780029";
				m1fy = "0.000000";
				sort = 40505;
				m2widx = 1;
				grade = 132203;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 40501;
				goal = 40504;
				m0fz = "-3903.439941";
				m0widx = 1;
				m1ttip = 40516;
				m2fy = "0.000000";
				m2pat = 0;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "-3906.840088";
				m2fx = "5844.180176";
				m2ttip = 40517;
				m0fx = "5914.189941";
				m0ttip = 40515;
				title = 40502;
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
				cnt1 = 0;
				eitype = 3;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 40508;
			gtype = 2;
			oklnk = 2;
			area = 40501;
			goal = 40504;
			sort = 40505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 40502;
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
				icnt2 = 255;
				iprob2 = "1.000000";
				icnt1 = 255;
				iidx0 = 633;
				iidx2 = 635;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
				icnt0 = 255;
				iidx1 = 634;
				iprob0 = "1.000000";
			}
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

