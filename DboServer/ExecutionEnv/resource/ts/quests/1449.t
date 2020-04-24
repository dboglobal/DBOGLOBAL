CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1449;
	title = 144902;

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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 144908;
			gtype = 2;
			oklnk = 2;
			area = 144901;
			goal = 144904;
			sort = 144905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 144902;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 144909;
				ctype = 1;
				idx = 5531101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 144908;
				gtype = 2;
				area = 144901;
				goal = 144904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 144905;
				stype = 2;
				taid = 1;
				title = 144902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 144914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 144901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "8;";
				widx = 2;
			}
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
			stdiag = 144907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 46;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1457;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5531101;";
			}
		}
	}
}

