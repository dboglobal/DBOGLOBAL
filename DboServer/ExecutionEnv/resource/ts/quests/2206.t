CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2206;
	title = 220602;

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
				conv = 220609;
				ctype = 1;
				idx = 8411121;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 220608;
				gtype = 1;
				area = 220601;
				goal = 220604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 220605;
				stype = 2;
				taid = 1;
				title = 220602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 220607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSClickNPC
			{
				npcidx = "8411121;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2207;";
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
			cont = 220608;
			gtype = 1;
			oklnk = 2;
			area = 220601;
			goal = 220604;
			sort = 220605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 220602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 220614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 220601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4111306;";
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

