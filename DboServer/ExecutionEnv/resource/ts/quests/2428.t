CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2428;
	title = 242802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContUsrSel
		{
			cancellnk = 3;
			cid = 4;
			lilnk = 255;
			ndesc0 = 242820;
			uspt = 3;
			desc = 242825;
			nid2 = 102;
			type = 0;
			ust = 1;
			idx = 3142304;
			ndesc2 = 242822;
			nid1 = 101;
			prelnk = "3;";
			ndesc1 = 242821;
			nid0 = 100;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "4;2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "3142304;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 242809;
				ctype = 1;
				idx = 3142304;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 242808;
				gtype = 3;
				area = 242801;
				goal = 242804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 242805;
				stype = 1;
				taid = 1;
				title = 242802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 102;
			rwdzeny = 0;
			desc = 242824;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 242803;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142304;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 242807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 53;
			}
			CDboTSClickNPC
			{
				npcidx = "3142304;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 242823;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 242801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142304;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "102;101;100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 242808;
			gtype = 3;
			oklnk = 2;
			area = 242801;
			goal = 242804;
			sort = 242805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 242802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 242814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 242802;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142304;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

