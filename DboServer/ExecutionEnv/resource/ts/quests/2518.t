CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 8;
	rq = 0;
	tid = 2518;
	title = 251802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 251809;
				ctype = 1;
				idx = 3173112;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 251808;
				gtype = 1;
				area = 251801;
				goal = 251804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 251805;
				stype = 2;
				taid = 1;
				title = 251802;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 251807;
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
				npcidx = "3173112;";
			}
			CDboTSCheckClrQst
			{
				and = "2467;";
				flink = 1;
				flinknextqid = "2519;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 251808;
			gtype = 1;
			oklnk = 2;
			area = 251801;
			goal = 251804;
			sort = 251805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 251802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 251814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 251801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181108;";
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

