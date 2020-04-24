CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1042;
	title = 104202;

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
				conv = 104209;
				ctype = 1;
				idx = 8031201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 104208;
				gtype = 1;
				area = 104201;
				goal = 104204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 104205;
				stype = 2;
				taid = 1;
				title = 104202;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 104214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 104201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031401;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 104208;
			gtype = 1;
			oklnk = 2;
			area = 104201;
			goal = 104204;
			sort = 104205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 104202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 104207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 38;
			}
			CDboTSCheckClrQst
			{
				and = "1041;";
				flink = 1;
				flinknextqid = "1043;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "8031201;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

