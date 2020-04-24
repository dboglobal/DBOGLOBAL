CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2973;
	title = 297302;

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
				conv = 297309;
				ctype = 1;
				idx = 4531104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1540;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 297308;
				m2fz = "-4645.779785";
				m3widx = 1;
				sort = 297305;
				m2widx = 1;
				grade = 132203;
				m3fx = "3523.179932";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				area = 297301;
				goal = 297304;
				m0fz = "-4684.879883";
				m0widx = 1;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4539.339844";
				m3ttip = 297317;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m2fx = "3438.610107";
				m2ttip = 297316;
				m0fx = "3676.010010";
				m0ttip = 297315;
				m3pat = 2;
				title = 297302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 297314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 297301;
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
				npcidx = "4531104;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 297308;
			gtype = 2;
			oklnk = 2;
			area = 297301;
			goal = 297304;
			sort = 297305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 297302;
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
			stdiag = 297307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "4531104;";
			}
			CDboTSCheckClrQst
			{
				and = "2971;2972;";
				flink = 1;
				flinknextqid = "2974;";
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
				iprob0 = "1.000000";
				iidx0 = 1540;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 297307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

