CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 278;
	title = 27802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 124;
			elnk = 255;
			nextlnk = 254;
			prelnk = "123;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 482;
				taid = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 123;
			prelnk = "122;121;";
			nolnk = 254;
			rm = 0;
			yeslnk = 124;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 482;
			}
		}
		CDboTSContGCond
		{
			cid = 121;
			prelnk = "101;";
			nolnk = 123;
			rm = 0;
			yeslnk = 122;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 481;
			}
		}
		CDboTSContGAct
		{
			cid = 122;
			elnk = 255;
			nextlnk = 123;
			prelnk = "121;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 481;
				taid = 255;
				type = 1;
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
			prelnk = "124;123;";
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
			stdiag = 27807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 13;
			}
			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
			CDboTSCheckClrQst
			{
				and = "274;";
				flink = 1;
				flinknextqid = "288;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 121;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 27814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 27801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "4511503;";
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
				conv = 27809;
				ctype = 1;
				idx = 4511503;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 1;
				idx0 = 481;
				cnt0 = 6;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 482;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 27808;
				m3widx = 1;
				m1fy = "0.000000";
				sort = 27805;
				m2widx = 1;
				grade = 132203;
				m1pat = 1;
				m1widx = 6;
				m3fx = "5181.629883";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 27801;
				goal = 27804;
				m0fz = "-2356.270020";
				m0widx = 1;
				m1ttip = 27816;
				m2fy = "0.000000";
				m2pat = 0;
				m3fz = "-2370.040039";
				m3ttip = 27815;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "998.979980";
				m2fx = "4361.000000";
				m2ttip = 27817;
				m0fx = "4914.430176";
				m0ttip = 27815;
				m3pat = 2;
				title = 27802;
				m1fx = "-574.539978";
				m2fz = "-3444.000000";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 27808;
			gtype = 2;
			oklnk = 2;
			area = 27801;
			goal = 27804;
			sort = 27805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 27802;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 482;
				taid = 255;
				type = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 482;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 482;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 481;
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
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 3;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 481;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;3;";
			type = 0;
		}
	}
}

