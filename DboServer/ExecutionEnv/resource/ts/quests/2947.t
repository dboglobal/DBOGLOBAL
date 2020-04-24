CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2947;
	title = 294702;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 294709;
				ctype = 1;
				idx = 4511313;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 7;
				eitype = 1;
				idx0 = 1532;
				cnt0 = 8;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 1564;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 294708;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "3719.889893";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "3318.899902";
				m4ttip = 294716;
				area = 294701;
				goal = 294704;
				m0fz = "-4031.639893";
				m0widx = 1;
				m1ttip = 294715;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-3815.929932";
				m3ttip = 294716;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3807.479980";
				m2fx = "3685.179932";
				m2ttip = 294716;
				m4widx = 1;
				m0fx = "3661.889893";
				m0ttip = 294715;
				m3pat = 2;
				m4pat = 2;
				title = 294702;
				m1fx = "3701.239990";
				m2fz = "-4051.649902";
				sort = 294705;
				m4fz = "-4141.810059";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 294714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 294701;
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
				npcidx = "4511313;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 294708;
			gtype = 2;
			oklnk = 2;
			area = 294701;
			goal = 294704;
			sort = 294705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 294702;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 294707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4511313;";
			}
			CDboTSCheckClrQst
			{
				and = "2946;";
				flink = 1;
				flinknextqid = "2948;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iidx1 = 1564;
				iprob0 = "1.000000";
				icnt1 = 255;
				iidx0 = 1532;
				iprob1 = "1.000000";
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 294707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

