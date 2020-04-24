CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3763;
	title = 376302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 3763;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSRcvSvrEvt
			{
				id = 1174;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 3763;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 376307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3183301;";
			}
			CDboTSCheckClrQst
			{
				and = "3762;";
				flink = 0;
				flinknextqid = "3764;";
				not = 0;
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
				conv = 376309;
				ctype = 1;
				idx = 3183301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 376308;
				area = 376301;
				goal = 376304;
				m0fz = "-5953.479980";
				m0widx = 15;
				scitem = -1;
				sort = 376305;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "-3966.709961";
				m0ttip = 376315;
				rwd = 100;
				taid = 1;
				title = 376302;
				gtype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 376314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 376301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183301;";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 376308;
			gtype = 2;
			oklnk = 2;
			area = 376301;
			goal = 376304;
			sort = 376305;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 376302;
		}
	}
}

